#include "LazerManager.h"
#include "CubeLazer.h"
#include "NormalLazer.h"
#include <string>
#include <cassert>

namespace
{
	const std::string cube_lazer_model_file_path = "Data/Model/MV1/CubeShot.mv1";
	const std::string normal_lazer_model_file_path = "Data/Model/MV1/NormalLazer.mv1";
}

LazerManager::LazerManager()
{
	lazerModelHanldeTable_[LazerType::CUBE] = MV1LoadModel(cube_lazer_model_file_path.c_str());
	lazerModelHanldeTable_[LazerType::NORMAL] = MV1LoadModel(normal_lazer_model_file_path.c_str());

	for (auto& lazerModelHandle : lazerModelHanldeTable_)
	{
		assert(lazerModelHandle.second != -1);
	}
}

LazerManager::~LazerManager()
{
	for (auto& lazerModelHandle : lazerModelHanldeTable_)
	{
		MV1DeleteModel(lazerModelHandle.second);
	}
}

void LazerManager::Create(LazerType lazerType, const VECTOR pos, const VECTOR vec, const VECTOR rot)
{
	LazerData data;
	data.type = lazerType;

	switch (lazerType)
	{
	case LazerType::CUBE:
		data.pLazer = std::make_shared<CubeLazer>(lazerModelHanldeTable_[lazerType]);
		break;

	case LazerType::NORMAL:
		data.pLazer = std::make_shared<NormalLazer>(lazerModelHanldeTable_[lazerType]);
		break;

	default:
		assert(0);
	}
	pLazeres_.push_back(data);
	pLazeres_.back().pLazer->Fire(pos, vec, rot);
}

void LazerManager::Update()
{
	// 不要になったレーザーの削除
	auto rmIt = std::remove_if(pLazeres_.begin(), pLazeres_.end(), [](const LazerData lazer)
		{
			return !lazer.pLazer->GetIsEnabled();
		});
	pLazeres_.erase(rmIt, pLazeres_.end());

	for (auto& lazer : pLazeres_)
	{
		lazer.pLazer->Update();
	}
}

void LazerManager::Draw()
{
	for (auto& lazer : pLazeres_)
	{
		lazer.pLazer->Draw();
	}
}

std::list<LazerData> LazerManager::GetLazeres() const
{
	return pLazeres_;
}
