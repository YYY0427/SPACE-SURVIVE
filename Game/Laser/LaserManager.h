#pragma once
#include "LaserBase.h"
#include <list>
#include <memory>
#include <unordered_map>

struct LaserData
{
	std::shared_ptr<LaserBase> pLaser;
	LaserType type;
};

class LaserManager
{
public:
	LaserManager();
	~LaserManager();

	void Create(LaserType laserType, VECTOR* pos, VECTOR* vec, int fireFrameCount = 1.0f, int chareFrameTime = 100);

	void Update();
	void Draw();

	/// <summary>
	/// ���[�U�[�𔽎�
	/// </summary>
	/// <param name="pos">���˂��郌�[�U�[�𔭎˂���ʒu</param>
	/// <param name="norm">�V�[���h�̖@�����</param>
	void Reflect(const VECTOR pos, const VECTOR vec, const VECTOR normal);

	// ���˃��[�U�[�̍폜
	void DeleteReflectLaser();

	// �p�����[�U�[�̍폜
	void DeleteContinueLaser();

	// �S�Ẵ��[�U�[�̍폜
	void GraduallyAlphaDeleteAllLaser();

	// ���[�U�[�̎擾
	const std::list<LaserData>& GetLazeres() const;

private:
	std::list<LaserData> pLaseres_;
	std::unordered_map<LaserType, int> laserModelHanldeTable_;
};