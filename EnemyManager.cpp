#include "EnemyManager.h"
#include "Enemy.h"

namespace
{
	constexpr float model_scale = 50.0f;
	constexpr float model_scale_2 = 80.0f;

	constexpr VECTOR model_pos = {3897, 1415, 60};
	constexpr VECTOR model_pos_2 = { -3926, 1415, 110 };
}

EnemyManager::EnemyManager(Player& pPlayer) :
	pPlayer_(pPlayer)
{
}

EnemyManager::~EnemyManager()
{
}

void EnemyManager::Update()
{
	float vecZ = GetRand(10) - 5;
	float vecY = GetRand(10) - 5;

	float scale;
	if (GetRand(1) == 0)
	{
		scale = model_scale;
	}
	else
	{
		scale = model_scale_2;
	}

	static int timer = 0;
	static bool isWitch = false;
	timer++;
	if (timer % 10 == 0)
	{
		if(!isWitch)
		{
			pEnemies_.push_back(std::make_shared<Enemy>(model_pos, VGet(-5.0f, vecY, vecZ), scale, pPlayer_));
			isWitch = true;
		}
		else
		{
			pEnemies_.push_back(std::make_shared<Enemy>(model_pos_2, VGet(5.0f, vecY, vecZ), scale, pPlayer_));
			isWitch = false;
		}
	}

	for (auto& enemies : pEnemies_)
	{
		enemies->Update();
	}
}

void EnemyManager::Draw()
{
	for (auto& enemies : pEnemies_)
	{
		enemies->Draw();
	}
}

std::vector<std::shared_ptr<Enemy>> EnemyManager::GetEnemies()
{
	return pEnemies_;
}
