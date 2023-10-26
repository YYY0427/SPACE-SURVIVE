#include "EnemyManager.h"
#include <string>
#include <DxLib.h>
#include <cassert>

namespace
{
	const std::string boss_enemy_model_file_path = "Data/Model/MV1/SpaceBattleship.mv1";
}

EnemyManager::EnemyManager()
{
	modelHandleTable_[EnemyType::NOMAL] = MV1LoadModel(boss_enemy_model_file_path.c_str());

	for (auto& handle : modelHandleTable_)
	{
		assert(handle.second != -1);
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