#pragma once
#include <memory>
#include <DxLib.h>

// �v���g�^�C�v�錾
class Model;

// �X�J�C�h�[��
class SkyDome
{
public:
	// �R���X�g���N�^
	SkyDome(VECTOR initPos);

	// �f�X�g���N�^
	virtual ~SkyDome();

	// �X�V
	void Update();

	// �`��
	void Draw();
private:
	// ���f��
	std::shared_ptr<Model> pModel_;
};