#include "EnemyManager.h"
#include "BossEnemy.h"
#include "NormalEnemy.h"
#include "EnemyBase.h"
#include "../Player.h"
#include "../common.h"
#include "../Util/Debug.h"
#include "../Util/DrawFunctions.h"
#include "../UI/Warning.h"
#include <string>
#include <DxLib.h>
#include <cassert>

namespace
{
	// ファイルの階層
	const std::string model_file_hierarchy = "Data/Model/MV1/";

	// ファイルの拡張子
	const std::string model_file_extension = ".mv1";

	// ファイル名
	const std::string normal_enemy_model_file_name = "NormalEnemy2";
	const std::string boss_enemy_model_file_name = "BossEnemy";

	// 何フレーム間、ボス出現のWARNING!!を描画するか
	constexpr int warning_ui_draw_frame = 60 * 1;

	// ゲーム開始から何フレーム経過したらボスを出現させるか
	constexpr int boss_create_frame = 60 * 10;
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

	// 雑魚敵のインスタンスの作成
	pEnemies_.push_back(std::make_shared<NormalEnemy>(modelHandleTable_[EnemyType::NOMAL], pPlayer, pLazerManager));
}

EnemyManager::~EnemyManager()
{
	for (auto& handle : modelHandleTable_)
	{
		MV1DeleteModel(handle.second);
	}
}

void EnemyManager::Update(int time)
{
	// 存在していない敵の削除
	DeleteNotEnabledEnemy();

	if (boss_create_frame < time &&
		!isCreateBossEnemy_)
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

	Debug::Log("time", time / 60);

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
		pEnemies_.push_back(
			std::make_shared<BossEnemy>(modelHandleTable_[EnemyType::BOSS],
				pPlayer_,
				pLazerManager_));

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