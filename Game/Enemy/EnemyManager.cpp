#include "EnemyManager.h"
#include "BossEnemy.h"
#include "NormalEnemy.h"
#include "../common.h"
#include "../Util/Debug.h"
#include "../Util/DrawFunctions.h"
#include "../UI/HpBar.h"
#include <string>
#include <DxLib.h>
#include <cassert>

namespace
{
	const std::string normal_enemy_model_file_path = "Data/Model/MV1/SpaceBattleship.mv1";
	const std::string boss_enemy_model_file_path = "Data/Model/MV1/SpaceBattleship.mv1";
	
	constexpr float max_hp = 100.0f;
	constexpr int hp_bar_side_space = 300;
	constexpr int hp_bar_start_y = 50;
	constexpr int hp_bar_height =  30;
	constexpr float aim_hp_speed = 0.09f;
}

EnemyManager::EnemyManager(std::shared_ptr<Player> pPlayer, std::shared_ptr<LazerManager> pLazerManager, UnityGameObject bossEnemyData, std::vector<UnityGameObject> normalEnemyData) :
	hp_(max_hp),
	isRepel_(false)
{
	modelHandleTable_[EnemyType::NOMAL] = my::MyLoadModel(normal_enemy_model_file_path.c_str());
	modelHandleTable_[EnemyType::BOSS] = my::MyLoadModel(boss_enemy_model_file_path.c_str());

	pEnemies_.push_back(std::make_shared<BossEnemy>(modelHandleTable_[EnemyType::BOSS], pPlayer, pLazerManager, bossEnemyData));
	for (auto& enemyData : normalEnemyData)
	{
		pEnemies_.push_back(std::make_shared<NormalEnemy>(modelHandleTable_[EnemyType::NOMAL], pPlayer, pLazerManager, enemyData));
	}

	pHpBar_ = std::make_unique<HpBar>(max_hp);
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

	pHpBar_->Update(aim_hp_speed);

	CheckRunAllEnemy();
}

void EnemyManager::Draw()
{
	for (auto& enemy : pEnemies_)
	{
		enemy->Draw();
	}
	pHpBar_->Draw(hp_bar_side_space, hp_bar_start_y, hp_bar_height);

	Debug::Log("“G‚ÌHP", hp_);
}

const std::list<std::shared_ptr<EnemyBase>>& EnemyManager::GetEnemies() const
{
	return pEnemies_;
}

void EnemyManager::OnDamage(float damage)
{
	hp_ -= damage;
	pHpBar_->OnDamage(hp_);
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