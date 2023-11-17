#include "EnemyManager.h"
#include "BossEnemy.h"
#include "NormalEnemy.h"
#include "EnemyBase.h"
#include "../Player.h"
#include "../common.h"
#include "../Util/Debug.h"
#include "../Util/DrawFunctions.h"
#include "../Util/DataReaderFromUnity.h"
#include "../UI/Warning.h"
#include <string>
#include <DxLib.h>
#include <cassert>

namespace
{
	const std::string model_file_hierarchy = "Data/Model/MV1/";
	const std::string model_file_extension = ".mv1";

	const std::string normal_enemy_model_file_name = "NormalEnemy2";
	const std::string boss_enemy_model_file_name = "BossEnemy";

	// 何フレーム間、ボス出現のWARNING!!を描画するか
	constexpr int warning_ui_draw_frame = 60 * 6;
}

EnemyManager::EnemyManager(std::shared_ptr<Player> pPlayer, std::shared_ptr<LazerManager> pLazerManager) :
	pPlayer_(pPlayer),
	pLazerManager_(pLazerManager),
	isCreateBossEnemy_(false),
	pWarning_(nullptr),
	updateFunc_(&EnemyManager::NormalUpdate)
{
	// モデルのロード
	std::string normalEnemyFilePath = model_file_hierarchy + normal_enemy_model_file_name + model_file_extension;
	modelHandleTable_[EnemyType::NOMAL] = my::MyLoadModel(normalEnemyFilePath.c_str());

	std::string bossEnemyFilePath = model_file_hierarchy + boss_enemy_model_file_name + model_file_extension;
	modelHandleTable_[EnemyType::BOSS] = my::MyLoadModel(bossEnemyFilePath.c_str());

	// インスタンスの作成
	auto& data = DataReaderFromUnity::GetInstance();
	for (auto& enemyData : data.GetData(normal_enemy_model_file_name))
	{
		pEnemies_.push_back(std::make_shared<NormalEnemy>(modelHandleTable_[EnemyType::NOMAL], pPlayer, pLazerManager, enemyData));
	}
}

EnemyManager::~EnemyManager()
{
	for (auto& handle : modelHandleTable_)
	{
		MV1DeleteModel(handle.second);
	}
}

void EnemyManager::Update()
{
	// 存在していない敵の削除
	DeleteNotEnabledEnemy();

	auto& data = DataReaderFromUnity::GetInstance();
	VECTOR toPlayerVec = VSub(pPlayer_->GetPos(), data.GetData(boss_enemy_model_file_name).front().pos);
	float distance = VSize(toPlayerVec);
	if (distance < 5000.0f && !isCreateBossEnemy_)
	{
		// ボスまで到着したので敵をすべて削除
		for (auto& enemy : pEnemies_)
		{
			enemy->Delete();
		}

		isCreateBossEnemy_ = true;

		pWarning_ = std::make_unique<Warning>(warning_ui_draw_frame);

		// updateをボス登場のupdateに切り替え
		updateFunc_ = &EnemyManager::CreateBossEnemyUpdate;
	}

	Debug::Log("distance", distance);

	(this->*updateFunc_)();
}	

void EnemyManager::Draw()
{
	for (auto& enemy : pEnemies_)
	{
		enemy->Draw();
	}

	if (pWarning_)
	{
		pWarning_->Draw();
	}
}

const std::list<std::shared_ptr<EnemyBase>>& EnemyManager::GetEnemies() const
{
	return pEnemies_;
}

void EnemyManager::NormalUpdate()
{
	for (auto& enemy : pEnemies_)
	{
		enemy->Update();
	}
}

void EnemyManager::CreateBossEnemyUpdate()
{
	pWarning_->Update();

	if (pWarning_->IsEnd())
	{
		// 敵ボスのインスタンス生成
		auto& data = DataReaderFromUnity::GetInstance();
		pEnemies_.push_back(
			std::make_shared<BossEnemy>(modelHandleTable_[EnemyType::BOSS],
				pPlayer_,
				pLazerManager_,
				data.GetData(boss_enemy_model_file_name).front()));

		updateFunc_ = &EnemyManager::NormalUpdate;
	}
}

void EnemyManager::DeleteNotEnabledEnemy()
{
	// 不要になった敵の削除
	auto rmIt = std::remove_if(pEnemies_.begin(), pEnemies_.end(), [](const std::shared_ptr<EnemyBase> enemy)
		{
			return !enemy->IsEnabled();
		});
	pEnemies_.erase(rmIt, pEnemies_.end());
}