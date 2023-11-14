#include "EnemyManager.h"
#include "BossEnemy.h"
#include "NormalEnemy.h"
#include "EnemyBase.h"
#include "../common.h"
#include "../Util/Debug.h"
#include "../Util/DrawFunctions.h"
#include "../Util/DataReaderFromUnity.h"
#include <string>
#include <DxLib.h>
#include <cassert>

namespace
{
	const std::string model_file_hierarchy = "Data/Model/MV1/";
	const std::string model_file_extension = ".mv1";

	const std::string normal_enemy_model_file_name = "NormalEnemy2";
	const std::string boss_enemy_model_file_name = "SpaceBattleship";
}

EnemyManager::EnemyManager(std::shared_ptr<Player> pPlayer, std::shared_ptr<LazerManager> pLazerManager)
{
	// モデルのロード
	std::string normalEnemyFilePath = model_file_hierarchy + normal_enemy_model_file_name + model_file_extension;
	modelHandleTable_[EnemyType::NOMAL] = my::MyLoadModel(normalEnemyFilePath.c_str());

	std::string bossEnemyFilePath = model_file_hierarchy + boss_enemy_model_file_name + model_file_extension;
	modelHandleTable_[EnemyType::BOSS] = my::MyLoadModel(bossEnemyFilePath.c_str());

	// インスタンスの作成
	auto& data = DataReaderFromUnity::GetInstance();
	pEnemies_.push_back(std::make_shared<BossEnemy>(modelHandleTable_[EnemyType::BOSS], pPlayer, pLazerManager, data.GetData(boss_enemy_model_file_name).front()));
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
	// 不要になった敵の削除
	auto rmIt = std::remove_if(pEnemies_.begin(), pEnemies_.end(), [](const std::shared_ptr<EnemyBase> enemy)
		{
			return !enemy->GetIsEnabled();
		});
	pEnemies_.erase(rmIt, pEnemies_.end());

	for (auto& enemy : pEnemies_)
	{
		// カメラ内に存在したら
	//	if (!CheckCameraViewClip(enemy->GetPos()))
		{
			enemy->Update();
		}
	}
}	

void EnemyManager::Draw()
{
	for (auto& enemy : pEnemies_)
	{
		enemy->Draw();
	}
}

const std::list<std::shared_ptr<EnemyBase>>& EnemyManager::GetEnemies() const
{
	return pEnemies_;
}