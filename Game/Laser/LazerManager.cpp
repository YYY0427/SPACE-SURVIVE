#include "LazerManager.h"
#include "CubeLazer.h"
#include "NormalLazer.h"
#include "ReflectLaser.h"
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

void LazerManager::Create(LaserType laserType, VECTOR* pos, VECTOR* vec, float effectSpeed)
{
	LaserData data;
	data.type = laserType;

	switch (laserType)
	{
	case LaserType::CUBE:
		data.pLaser = std::make_shared<CubeLazer>(laserModelHanldeTable_[laserType], *pos, *vec);
		break;

	case LaserType::NORMAL:
		data.pLaser = std::make_shared<NormalLazer>(laserModelHanldeTable_[laserType], pos, vec, effectSpeed, false);
		break;

	case LaserType::CONTINUE_NORMAL:
		data.pLaser = std::make_shared<NormalLazer>(laserModelHanldeTable_[LaserType::NORMAL], pos, vec, effectSpeed, true);
		break;

	default:
		assert(0);
	}
	pLaseres_.push_back(data);
}

void LazerManager::Update()
{
	// �s�v�ɂȂ������[�U�[�̍폜
	pLaseres_.remove_if([](LaserData data){ return !data.pLaser->IsEnabled(); });

	auto it = std::find_if(
		pLaseres_.begin(), pLaseres_.end(),
		[](LaserData data) { return data.pLaser->IsReflect(); });

	if (it == pLaseres_.end())
	{
		DeleteReflectLaser();
	}

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

void LazerManager::Reflect(const VECTOR pos, const VECTOR vec, const VECTOR normal)
{
	auto it = std::find_if(
		pLaseres_.begin(), pLaseres_.end(), 
		[](LaserData data) { return data.type == LaserType::REFLECT; });

	// ���˃x�N�g���̍쐬
	VECTOR inversionVec = VScale(vec, -1);
	float dot = VDot(inversionVec, normal);
	dot *= 2.0f;
	VECTOR normVec = VScale(normal, dot);
	VECTOR reflectVec = VAdd(vec, normVec);

	// ���˃��[�U�[�����݂��邩
	if (it == pLaseres_.end()) 
	{
		// ������Ȃ�����

		// ���˃��[�U�[���쐬
		LaserData data;
		data.type = LaserType::REFLECT;
		data.pLaser = std::make_shared<ReflectLaser>(
			laserModelHanldeTable_[LaserType::NORMAL], 
			pos, reflectVec);
		pLaseres_.push_back(data);
	}
	else 
	{
		// ��������
		it->pLaser->ReflectLaserUpdate(pos, reflectVec);
	}
}

void LazerManager::DeleteReflectLaser()
{
	pLaseres_.remove_if([](LaserData data) { return data.type == LaserType::REFLECT; });
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
