#include "RockManager.h"
#include "Rock.h"
#include "Meteor.h"
#include <string>

namespace
{
	// モデルのファイルパス
	std::string rock_data_file_path = "Data/Model/Rock.mv1";
	std::string meteor_data_file_path = "Data/Model/Meteor.mv1";
}

RockManager::RockManager(std::vector<UnityGameObject> rockData, std::vector<UnityGameObject> meteorData, std::shared_ptr<Player> pPlayer)
{
	// モデルのロード
	handle_ = MV1LoadModel(rock_data_file_path.c_str());
	handle2_ = MV1LoadModel(meteor_data_file_path.c_str());

	for (auto& data : rockData)
	{
		pRocks_.push_back(std::make_shared<Rock>(handle_, pPlayer, data));
	}
	for (auto& data : meteorData)
	{
		pRocks_.push_back(std::make_shared<Meteor>(handle2_, pPlayer, data));
	}
}

RockManager::~RockManager()
{
	MV1DeleteModel(handle_);
}

void RockManager::Update()
{
	for (auto& enemies : pRocks_)
	{
		enemies->Update();
	}
	CheckEnabled();
}

void RockManager::Draw()
{
	for (auto& enemies : pRocks_)
	{
		enemies->Draw();
	}
}

void RockManager::CheckEnabled()
{
	auto enemies = std::remove_if(pRocks_.begin(), pRocks_.end(), [](const std::shared_ptr<RockBase>& rocks)
		{
			return !rocks->GetIsEnabled();
		});
	pRocks_.erase(enemies, pRocks_.end());
}

std::list<std::shared_ptr<RockBase>> RockManager::GetRocks()
{
	return pRocks_;
}
