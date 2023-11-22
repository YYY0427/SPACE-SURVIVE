#pragma once
#include <DxLib.h>
#include <memory>
#include "../Util/Model.h"

enum class LaserType
{
	NORMAL, 
	CONTINUE_NORMAL,
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
	/// ���[�U�[�𔽎�
	/// </summary>
	/// <param name="pos">���˂��郌�[�U�[�𔭎˂���ʒu</param>
	/// <param name="norm">�V�[���h�̖@�����</param>
	virtual void Refrect(const VECTOR pos, const VECTOR normal){};


	void Delete();

	// �J�������ɑ��݂��邩
	// ���݂��Ȃ�������폜����
	virtual void ConfirmDelete() = 0;

	bool GetIsEnabled() const;

	int GetModelHandle() const;

	bool GetIsRefrect() const;

protected:
	std::unique_ptr<Model> pModel_;

	VECTOR pos_;
	VECTOR* firePos_;
	VECTOR vec_;
	VECTOR rot_;
	VECTOR scale_;
	VECTOR endScale_;

	bool isEnabled_;

	// ���ˌ�̃��[�U�[��
	bool isRefrect_;
};