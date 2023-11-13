#include "LazerManager.h"
#include "CubeLazer.h"
#include "NormalLazer.h"
#include "../Util/DrawFunctions.h"
#include "../Util/Debug.h"
#include <string>
#include <cassert>

namespace
{
	const std::string cube_lazer_model_file_path = "Data/Model/MV1/CubeShot.mv1";
	const std::string normal_lazer_model_file_path = "Data/Model/MV1/NormalLazer.mv1";
}

LazerManager::LazerManager()
{
	lazerModelHanldeTable_[LazerType::CUBE] = my::MyLoadModel(cube_lazer_model_file_path.c_str());
	lazerModelHanldeTable_[LazerType::NORMAL] = my::MyLoadModel(normal_lazer_model_file_path.c_str());
}

LazerManager::~LazerManager()
{
	for (auto& lazerModelHandle : lazerModelHanldeTable_)
	{
		MV1DeleteModel(lazerModelHandle.second);
	}
}

void LazerManager::Create(LazerType lazerType, VECTOR* pos, VECTOR* vec, VECTOR* enemyMoveVec)
{
	LazerData data;
	data.type = lazerType;

	switch (lazerType)
	{
	case LazerType::CUBE:
		data.pLazer = std::make_shared<CubeLazer>(lazerModelHanldeTable_[lazerType], *pos, *vec);
		break;

	case LazerType::NORMAL:
		data.pLazer = std::make_shared<NormalLazer>(lazerModelHanldeTable_[lazerType], pos, vec, enemyMoveVec);
		break;

	default:
		assert(0);
	}
	pLazeres_.push_back(data);
}

void LazerManager::Update(VECTOR scrollVec)
{
	// 不要になったレーザーの削除
	auto rmIt = std::remove_if(pLazeres_.begin(), pLazeres_.end(), [](const LazerData lazer)
		{
			return !lazer.pLazer->GetIsEnabled();
		});
	pLazeres_.erase(rmIt, pLazeres_.end());

	for (auto& lazer : pLazeres_)
	{
		lazer.pLazer->CheckInCamera();
		lazer.pLazer->Update(scrollVec);
	}
}

void LazerManager::Draw()
{
	for (auto& lazer : pLazeres_)
	{
		lazer.pLazer->Draw();
	}
}

const std::list<LazerData>& LazerManager::GetLazeres() const
{
	return pLazeres_;
}
