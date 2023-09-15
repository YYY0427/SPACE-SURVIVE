#include "Player.h"
#include "Util/InputState.h"
#include "Util/Model.h"
#include <string>

namespace
{
	// ���f���̃t�@�C���̃p�X
	string model_file_path = "Data/Model/UFO.mv1";
}

/// <summary>
///  �R���X�g���N�^
/// </summary>
Player::Player() : 
	pos_(VGet(0.0f, 0.0f, 0.0f))
{
	pModel_ = make_shared<Model>(model_file_path.c_str());
	pModel_->SetScale(VGet(10.0f, 10.0f, 10.0f));
}

/// <summary>
///  �f�X�g���N�^
/// </summary>
Player::~Player()
{
}

/// <summary>
/// �X�V
/// </summary>
void Player::Update()
{
	int up = InputState::IsXInputStick(XInputType::LEFT, XInputTypeStick::UP);
	int down = InputState::IsXInputStick(XInputType::LEFT, XInputTypeStick::DOWN);
	int left = InputState::IsXInputStick(XInputType::LEFT, XInputTypeStick::LEFT);
	int right = InputState::IsXInputStick(XInputType::LEFT, XInputTypeStick::RIGHT);

	pos_.z -= up;
	pos_.z += down;
	pos_.x -= left;
	pos_.x += right;

	// �ʒu���W�̐ݒ�
	pModel_->SetPos(pos_);

	// �����Ă�������̐ݒ�
//	pModel_->SetRot(VGet(0.0f, pCamera_->GetCameraYaw(), 0.0f));

	// �A�j���[�V������i�߂�
	pModel_->Update();
}

/// <summary>
/// �`��
/// </summary>
void Player::Draw()
{
	pModel_->Draw();
}