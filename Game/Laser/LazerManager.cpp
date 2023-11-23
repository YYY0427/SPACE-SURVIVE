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
		data.pLaser = std::make_shared<CubeLazer>(laserModelHanldeTable_[laserType], *pos, *vec);
		break;

	case LaserType::NORMAL:
		data.pLaser = std::make_shared<NormalLazer>(laserModelHanldeTable_[laserType], pos, vec, fireObjectMoveVec, false);
		break;

	case LaserType::CONTINUE_NORMAL:
		data.pLaser = std::make_shared<NormalLazer>(laserModelHanldeTable_[LaserType::NORMAL], pos, vec, fireObjectMoveVec, true);
		break;

	default:
		assert(0);
	}
	pLaseres_.push_back(data);
}

void LazerManager::Update()
{
	// 不要になったレーザーの削除
	pLaseres_.remove_if([](LaserData data){ return !data.pLaser->IsEnabled(); });

	for (auto& laser : pLaseres_)
	{
		laser.pLaser->ConfirmDelete();
		laser.pLaser->Update();
	}
}

void LazerManager::Draw()
{
	for (auto& laser : pLaseres_)
	{
		laser.pLaser->Draw();
	}
}

void LazerManager::DeleteContinueLaser()
{
	pLaseres_.remove_if(
		[](LaserData data)
		{ return data.type == LaserType::CONTINUE_NORMAL; });
}

const std::list<LaserData>& LazerManager::GetLazeres() const
{
	return pLaseres_;
}
