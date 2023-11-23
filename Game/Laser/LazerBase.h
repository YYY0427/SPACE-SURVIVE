#pragma once
#include <DxLib.h>
#include <memory>
#include "../Util/Model.h"

enum class LaserType
{
	NORMAL, 
	CONTINUE_NORMAL,
	CUBE,
	REFLECT
};

class LazerBase
{
public:
	LazerBase();
	virtual ~LazerBase();

	virtual void Update() = 0;
	virtual void Draw() = 0;

	// ���[�U�[���~�߂�
	virtual void Stop(const VECTOR pos){};

	void Delete();

	// �J�������ɑ��݂��邩
	// ���݂��Ȃ�������폜����
	virtual void ConfirmDelete() = 0;

	bool IsEnabled() const;

	int GetModelHandle() const;

	virtual VECTOR GetVec() const;

	virtual void ReflectLaserUpdate(VECTOR pos, VECTOR vec){}

protected:
	std::unique_ptr<Model> pModel_;

	VECTOR pos_;
	VECTOR* firePos_;
	VECTOR vec_;
	VECTOR rot_;
	VECTOR scale_;
	VECTOR endScale_;

	bool isEnabled_;
};