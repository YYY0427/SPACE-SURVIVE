#pragma once
#include <memory>
#include <DxLib.h>
#include "../Util/DataReaderFromUnity.h"

class Model;

class RockBase
{
public:
	// �R���X�g���N�^
	RockBase();

	// �f�X�g���N�^
	virtual ~RockBase();

	// �X�V
	virtual void Update() = 0;

	// �`��
	virtual void Draw();

	// ���f���n���h���̎擾
	int GetModelHandle() const;

	// ���݂��邩�ǂ����̎擾
	bool GetIsEnabled() const;

protected:
	// �|�C���^
	std::shared_ptr<Model> pModel_;

	// ���W
	VECTOR pos_;

	// ��]
	VECTOR rot_;

	// �x�N�g��
	VECTOR vec_;

	// ���݂��邩
	bool isEnabled_;
};