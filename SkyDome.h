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
	SkyDome();

	// �f�X�g���N�^
	virtual ~SkyDome();

	/// <summary>
	/// �X�V
	/// </summary>
	/// <param name="playerPos">�v���C���[�̈ʒu���</param>
	void Update(VECTOR playerPos);

	// �`��
	void Draw();
private:
	// ���f��
	std::shared_ptr<Model> pModel_;
};