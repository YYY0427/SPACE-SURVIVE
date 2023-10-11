#pragma once
#include <memory>
#include <DxLib.h>
#include "../Util/DataReaderFromUnity.h"

class Model;

class PlanetBase
{
public:
	// �R���X�g���N�^
	PlanetBase();

	// �f�X�g���N�^
	virtual ~PlanetBase();

	// �X�V
	virtual void Update() = 0;

	// �`��
	virtual void Draw() = 0;

	// ���f���n���h���̎擾
	int GetModelHandle() const;

protected:
	// �|�C���^
	std::shared_ptr<Model> pModel_;

	// ���W
	VECTOR pos_;

	// ��]
	VECTOR rot_;
};