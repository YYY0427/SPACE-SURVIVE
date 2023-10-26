#include "EnemyManager.h"
#include "BossEnemy.h"
#include "NormalEnemy.h"
#include <string>
#include <DxLib.h>
#include <cassert>

namespace
{
	const std::string normal_enemy_model_file_path = "Data/Model/MV1/SpaceBattleship.mv1";
	const std::string boss_enemy_model_file_path = "Data/Model/MV1/SpaceBattleship.mv1";
}

EnemyManager::EnemyManager(std::shared_ptr<Player> pPlayer, std::shared_ptr<LazerManager> pLazerManager, UnityGameObject bossEnemyData, std::vector<UnityGameObject> normalEnemyData)
{
	modelHandleTable_[EnemyType::NOMAL] = MV1LoadModel(normal_enemy_model_file_path.c_str());
	modelHandleTable_[EnemyType::BOSS] = MV1LoadModel(boss_enemy_model_file_path.c_str());

	for (auto& handle : modelHandleTable_)
	{
		assert(handle.second != -1);
	}

	pEnemies_.push_back(std::make_shared<BossEnemy>(modelHandleTable_[EnemyType::BOSS], pPlayer, pLazerManager, bossEnemyData));

	for (auto& enemyData : normalEnemyData)
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
	for (auto& enemy : pEnemies_)
	{
		enemy->Update();
	}
}

void EnemyManager::Draw()
{
	for (auto& enemy : pEnemies_)
	{
		enemy->Draw();
	}
}