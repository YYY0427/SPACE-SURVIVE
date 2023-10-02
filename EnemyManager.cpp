#include "EnemyManager.h"
#include "Enemy.h"

namespace
{
	constexpr VECTOR model_pos = { 3897, 1415, 60 };
	constexpr VECTOR model_pos_2 = { -3926, 1415, 110 };
}

EnemyManager::EnemyManager(std::vector<DataReaderFromUnity::UnityGameObject> data, std::shared_ptr<Player> pPlayer) :
	data_(data),
	pPlayer_(pPlayer)
{
}

EnemyManager::~EnemyManager()
{
	MV1DeleteModel(handle_);
}

void EnemyManager::Init()
{
	handle_ = MV1LoadModel("Data/Model/Meteor.mv1");

	for (auto& data : data_)
	{
		pEnemies_.push_back(std::make_shared<Enemy>(handle_, data.pos, VGet(-5.0f, 5, 5), 3, pPlayer_));
	}
}

void EnemyManager::Update()
{
	/*float vecZ = GetRand(10) - 5;
	float vecY = GetRand(10) - 5;

	float scale = (GetRand(5));

	static int timer = 0;
	static bool isWitch = false;
	timer++;
	if (timer % 100 == 0)
	{
		if(!isWitch)
		{
			pEnemies_.push_back(std::make_shared<Enemy>(handle_, model_pos, VGet(-5.0f, vecY, vecZ), scale, pPlayer_));
			isWitch = true;
		}
		else
		{
			pEnemies_.push_back(std::make_shared<Enemy>(handle_, model_pos_2, VGet(5.0f, vecY, vecZ), scale, pPlayer_));
			isWitch = false;
		}
	}*/

	for (auto& enemies : pEnemies_)
	{
		enemies->Update();
	}
	CheckEnabled();
}

void EnemyManager::Draw()
{
	for (auto& enemies : pEnemies_)
	{
		enemies->Draw();
	}
}

void EnemyManager::CheckEnabled()
{
	auto enemies = std::remove_if(pEnemies_.begin(), pEnemies_.end(), [](const std::shared_ptr<Enemy>& enemies)
		{
			return !enemies->GetIsEnabled();
		});
	pEnemies_.erase(enemies, pEnemies_.end());
}

std::list<std::shared_ptr<Enemy>> EnemyManager::GetEnemies()
{
	return pEnemies_;
}
