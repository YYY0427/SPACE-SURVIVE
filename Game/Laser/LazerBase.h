#pragma once
#include <DxLib.h>
#include <memory>
#include "../Util/Model.h"

enum class LazerType
{
	NORMAL, 
	CUBE,
};

class LazerBase
{
public:
	LazerBase();
	virtual ~LazerBase();

	virtual void Update() = 0;
	virtual void Draw() = 0;

	/// <summary>
	/// ���[�U�[�𔭎�
	/// </summary>
	/// <param name="pos">���ˏꏊ</param>
	virtual void Fire(const VECTOR pos, const VECTOR vec);

	void Delete();
	virtual void Refrect(VECTOR pos){};

	// �J�������ɑ��݂��邩
	// ���݂��Ȃ�������폜����
	virtual void CheckInCamera() = 0;

	bool GetIsEnabled() const;

	int GetModelHandle() const;

	bool GetIsRefrect() const;

protected:
	std::unique_ptr<Model> pModel_;

	VECTOR pos_;
	VECTOR vec_;
	VECTOR rot_;
	VECTOR scale_;

	bool isEnabled_;

	// ���ˌ�̃��[�U�[��
	bool isRefrect_;
};