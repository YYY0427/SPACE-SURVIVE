#include "SkyDome.h"
#include "Player.h"
#include "Util/Model.h"
#include <string>

namespace
{
	// �X�J�C�h�[���̃��f���̃t�@�C���p�X
	const std::string model_file_path = "Data/Model/MV1/SkyDome.mv1";

	// ���f���̊g�嗦
	constexpr float model_scale = 2000.0f;
}

// �R���X�g���N�^
SkyDome::SkyDome(VECTOR initPos)
{
	// �C���X�^���X�̍쐬
	pModel_ = std::make_shared<Model>(model_file_path);

	// ���f���̊g�嗦�̐ݒ�
	pModel_->SetScale(VGet(model_scale, model_scale, model_scale));

	// �ʒu���̐ݒ�
	pModel_->SetPos(initPos);
}

// �f�X�g���N�^
SkyDome::~SkyDome()
{
}

// �X�V
void SkyDome::Update()
{
}

// �`��
void SkyDome::Draw()
{
	pModel_->Draw();
}