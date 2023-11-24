#pragma once
#include "LazerBase.h"
#include <list>
#include <memory>
#include <unordered_map>

struct LaserData
{
	std::shared_ptr<LazerBase> pLaser;
	LaserType type;
};

class LazerManager
{
public:
	LazerManager();
	~LazerManager();

	void Create(LaserType laserType, VECTOR* pos, VECTOR* vec, float effectSpeed = 1.0f);

	void Update();
	void Draw();

	/// <summary>
	/// ���[�U�[�𔽎�
	/// </summary>
	/// <param name="pos">���˂��郌�[�U�[�𔭎˂���ʒu</param>
	/// <param name="norm">�V�[���h�̖@�����</param>
	void Reflect(const VECTOR pos, const VECTOR vec, const VECTOR normal);

	void DeleteReflectLaser();

	// �p�����[�U�[�̍폜
	void DeleteContinueLaser();

	const std::list<LaserData>& GetLazeres() const;

private:
	std::list<LaserData> pLaseres_;
	std::unordered_map<LaserType, int> laserModelHanldeTable_;

	VECTOR reflectLaserPos_;
	VECTOR reflectVec_;
};