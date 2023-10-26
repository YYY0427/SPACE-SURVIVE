#include "LazerManager.h"
#include "CubeLazer.h"
#include <string>
#include <cassert>

namespace
{
	const std::string cube_lazer_model_file_path = "Data/Model/MV1/CubeShot.mv1";
}

LazerManager::LazerManager()
{
	lazerModelHanldeTable_[LazerType::CUBE] = MV1LoadModel(cube_lazer_model_file_path.c_str());

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

void LazerManager::Create(LazerType lazerType, const VECTOR pos, const VECTOR vec)
{
	switch (lazerType)
	{
	case LazerType::CUBE:
		pLazeres_.push_back(std::make_unique<CubeLazer>(lazerModelHanldeTable_[LazerType::CUBE]));
		break;
	default:
		assert(0);
	}
	pLazeres_.back()->Fire(pos, vec);
}

void LazerManager::Update()
{
	// 不要になったレーザーの削除
	auto rmIt = std::remove_if(pLazeres_.begin(), pLazeres_.end(), [](const std::unique_ptr<LazerBase>& lazer)
		{
			return !lazer->GetIsEnabled();
		});
	pLazeres_.erase(rmIt, pLazeres_.end());

	for (auto& lazer : pLazeres_)
	{
		lazer->Update();
	}
}

void LazerManager::Draw()
{
	for (auto& lazer : pLazeres_)
	{
		lazer->Draw();
	}
}

const std::list<std::unique_ptr<LazerBase>>& LazerManager::GetLazeres() const
{
	return pLazeres_;
}
