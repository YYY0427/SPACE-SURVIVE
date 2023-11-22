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
	laserModelHanldeTable_[LaserType::CUBE] = my::MyLoadModel(cube_lazer_model_file_path.c_str());
	laserModelHanldeTable_[LaserType::NORMAL] = my::MyLoadModel(normal_lazer_model_file_path.c_str());
}

LazerManager::~LazerManager()
{
	for (auto& lazerModelHandle : laserModelHanldeTable_)
	{
		MV1DeleteModel(lazerModelHandle.second);
	}
}

void LazerManager::Create(LaserType laserType, VECTOR* pos, VECTOR* vec, VECTOR* fireObjectMoveVec)
{
	LaserData data;
	data.type = laserType;

	switch (laserType)
	{
	case LaserType::CUBE:
		data.pLazer = std::make_shared<CubeLazer>(laserModelHanldeTable_[laserType], *pos, *vec);
		break;

	case LaserType::NORMAL:
		data.pLazer = std::make_shared<NormalLazer>(laserModelHanldeTable_[laserType], pos, vec, fireObjectMoveVec, false);
		break;

	case LaserType::CONTINUE_NORMAL:
		data.pLazer = std::make_shared<NormalLazer>(laserModelHanldeTable_[laserType], pos, vec, fireObjectMoveVec, true);
		break;

	default:
		assert(0);
	}
	pLazeres_.push_back(data);
}

void LazerManager::Update()
{
	// 不要になったレーザーの削除
	auto rmIt = std::remove_if(pLazeres_.begin(), pLazeres_.end(), [](const LaserData lazer)
		{
			return !lazer.pLazer->GetIsEnabled();
		});
	pLazeres_.erase(rmIt, pLazeres_.end());

	for (auto& lazer : pLazeres_)
	{
		lazer.pLazer->ConfirmDelete();
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

const std::list<LaserData>& LazerManager::GetLazeres() const
{
	return pLazeres_;
}
