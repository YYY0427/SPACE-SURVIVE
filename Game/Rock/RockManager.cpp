#include "RockManager.h"
#include "Rock.h"
#include "Meteor.h"
#include <string>
#include <cassert>

namespace
{
	// ���f���̃t�@�C���p�X
	std::string rock_data_file_path = "Data/Model/MV1/Rock.mv1";
	std::string meteor_data_file_path = "Data/Model/MV1/Meteor.mv1";
}

RockManager::RockManager(std::vector<UnityGameObject> rockData, std::vector<UnityGameObject> meteorData)
{
	// ���f���̃��[�h
	handleTable_[RockType::ROCK] = MV1LoadModel(rock_data_file_path.c_str());
	handleTable_[RockType::METEOR] = MV1LoadModel(meteor_data_file_path.c_str());

	// 1�ł����f���̃��[�h�Ɏ��s������~�߂�
	for (auto& handle : handleTable_)
	{
		assert(handle.second != -1);
	}

	for (auto& data : rockData)
	{
		pRocks_.push_back(std::make_shared<Rock>(handleTable_[RockType::ROCK], data));
	}
	for (auto& data : meteorData)
	{
		pRocks_.push_back(std::make_shared<Meteor>(handleTable_[RockType::METEOR], data));
	}
}

RockManager::~RockManager()
{
	for (auto& handle : handleTable_)
	{
		MV1DeleteModel(handle.second);
	}
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
