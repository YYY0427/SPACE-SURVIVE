#include "EnemyManager.h"
#include "BossEnemy.h"
#include "NormalEnemy.h"
#include "../Util/Debug.h"
#include <string>
#include <DxLib.h>
#include <cassert>

namespace
{
	const std::string normal_enemy_model_file_path = "Data/Model/MV1/SpaceBattleship.mv1";
	const std::string boss_enemy_model_file_path = "Data/Model/MV1/SpaceBattleship.mv1";
	constexpr int max_hp = 10;
}

EnemyManager::EnemyManager(std::shared_ptr<Player> pPlayer, std::shared_ptr<LazerManager> pLazerManager, UnityGameObject bossEnemyData, std::vector<UnityGameObject> normalEnemyData) :
	hp_(max_hp),
	isRepel_(false)
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
	CheckRunAllEnemy();
}

void EnemyManager::Draw()
{
	for (auto& enemy : pEnemies_)
	{
		enemy->Draw();
	}
	Debug::Log("“G‚ÌHP", hp_);
}

const std::list<std::shared_ptr<EnemyBase>>& EnemyManager::GetEnemies() const
{
	return pEnemies_;
}

void EnemyManager::OnDamage(int damage)
{
	hp_ -= damage;
}

bool EnemyManager::GetIsRepel() const
{
	return isRepel_;
}

void EnemyManager::CheckRunAllEnemy()
{
	if (hp_ <= 0)
	{
		int count = 0;
		for (auto& enemy : pEnemies_)
		{
			if (enemy->Run())
			{
				count++;
			}
		}
		if (count == static_cast<int>(pEnemies_.size()))
		{
			isRepel_ = true;
		}
	}
}