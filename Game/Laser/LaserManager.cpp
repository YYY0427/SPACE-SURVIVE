#include "LaserManager.h"
#include "CubeLaser.h"
#include "Laser.h"
#include "ReflectLaser.h"
#include "../Util/DrawFunctions.h"
#include "../Util/Debug.h"
#include <string>
#include <cassert>

namespace
{
	const std::string normal_lazer_model_file_path = "Data/Model/Laser.mv1";
	const std::string cube_lazer_model_file_path = "Data/Model/CubeLaser.mv1";
}

LaserManager::LaserManager()
{
	laserModelHanldeTable_[LaserType::CUBE] = my::MyLoadModel(cube_lazer_model_file_path.c_str());
	laserModelHanldeTable_[LaserType::NORMAL] = my::MyLoadModel(normal_lazer_model_file_path.c_str());
}

LaserManager::~LaserManager()
{
	for (auto& lazerModelHandle : laserModelHanldeTable_)
	{
		MV1DeleteModel(lazerModelHandle.second);
	}
}

void LaserManager::Create(LaserType laserType, VECTOR* pos, VECTOR* vec, int fireFrameTime, int chareFrameTime)
{
	LaserData data;
	data.type = laserType;

	switch (laserType)
	{
	case LaserType::CUBE:
		data.pLaser = std::make_shared<CubeLaser>(laserModelHanldeTable_[laserType], *pos, *vec);
		break;

	case LaserType::NORMAL:
		data.pLaser = std::make_shared<Laser>(laserModelHanldeTable_[laserType], pos, vec, fireFrameTime, false, chareFrameTime);
		break;

	case LaserType::NORMAL_INFINITY:
		data.pLaser = std::make_shared<Laser>(laserModelHanldeTable_[LaserType::NORMAL], pos, vec, fireFrameTime, true, chareFrameTime);
		break;

	default:
		assert(0);
	}
	pLaseres_.push_back(data);
}

void LaserManager::Update()
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

void LaserManager::Draw()
{
	for (auto& laser : pLaseres_)
	{
		laser.pLaser->Draw();
	}
}

void LaserManager::Reflect(const VECTOR pos, const VECTOR vec, const VECTOR normal)
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
		// ���˃��[�U�[�̍X�V
		auto a = std::dynamic_pointer_cast<ReflectLaser>(it->pLaser);
		a->ReflectLaserUpdate(pos, reflectVec);
	}
}

void LaserManager::DeleteReflectLaser()
{
	pLaseres_.remove_if([](LaserData data) { return data.type == LaserType::REFLECT; });
}

void LaserManager::DeleteContinueLaser()
{
	pLaseres_.remove_if(
		[](LaserData data)
		{ return data.type == LaserType::NORMAL_INFINITY; });
}

void LaserManager::GraduallyAlphaDeleteAllLaser()
{
	for (auto& laser : pLaseres_)
	{
		laser.pLaser->GraduallyAlphaDelete();
	}
}

const std::list<LaserData>& LaserManager::GetLazeres() const
{
	return pLaseres_;
}
