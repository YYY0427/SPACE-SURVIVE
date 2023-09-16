#include "Player.h"
#include "Camera.h"
#include "Util/InputState.h"
#include "Util/Model.h"
#include <string>

namespace
{
	// ���f���̃t�@�C���̃p�X
	string model_file_path = "Data/Model/UFO.mv1";

	// �v���C���[�̈ړ���
	constexpr VECTOR player_vec_up{ 0, 0, -1 };
	constexpr VECTOR player_vec_down{ 0, 0, 1 };
	constexpr VECTOR player_vec_right{ 1, 0, 0 };
	constexpr VECTOR player_vec_left{ -1, 0, 0 };
}

/// <summary>
///  �R���X�g���N�^
/// </summary>
Player::Player() : 
	pos_(VGet(0.0f, 0.0f, 0.0f)),
	isMove_(false)
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

	// �v���C���[�̉�]�l���擾����
	VECTOR vect = MV1GetRotationXYZ(pModel_->GetModelHandle());

	// �J�����������Ă����������x�N�g���ϊ����Ĉړ����쐬
	VECTOR moveUp = VTransform(player_vec_up, MGetRotY(pCamera_->GetCameraYaw()));
	VECTOR moveDown = VTransform(player_vec_down, MGetRotY(pCamera_->GetCameraYaw()));
	VECTOR moveRight = VTransform(player_vec_right, MGetRotY(pCamera_->GetCameraYaw() + vect.x));
	VECTOR moveLeft = VTransform(player_vec_left, MGetRotY(pCamera_->GetCameraYaw() + vect.x));

	isMove_ = false;
	VECTOR moveVec_ = VGet(0, 0, 0);
	VECTOR moveVecX = VGet(0, 0, 0);
	VECTOR moveVecZ = VGet(0, 0, 0);
	if (up > 5)
	{
		moveVecZ = moveUp;
		isMove_ = true;
	}
	if (left > 5)
	{
		moveVecX = moveLeft;
		isMove_ = true;
	}
	if (down > 5)
	{
		moveVecZ = moveDown;
		isMove_ = true;
	}
	if (right > 5)
	{
		moveVecX = moveRight;
		isMove_ = true;
	}

	if (isMove_)
	{
		// x������z�����̃x�N�g���𑫂��Ĉړ��x�N�g�����쐬����
		moveVec_ = VAdd(moveVecZ, moveVecX);

		// ���K��
		moveVec_ = VNorm(moveVec_);

		// ���K�������x�N�g���Ƀv���C���[�̑��x��������
		moveVec_ = VScale(moveVec_, 2.0f);

		pos_ = VAdd(pos_, moveVec_);
	}
	if (InputState::IsPressed(InputType::RIZE))
	{
		pos_.y++;
	}

	// �ʒu���W�̐ݒ�
	pModel_->SetPos(pos_);

	// �����Ă�������̐ݒ�
	pModel_->SetRot(VGet(0.0f, pCamera_->GetCameraYaw(), 0.0f));

	// �A�j���[�V������i�߂�
	pModel_->Update();
}

/// <summary>
/// �`��
/// </summary>
void Player::Draw()
{
	pModel_->Draw();
	DrawFormatString(10, 80, 0xffffff, "playerPos = %.2f, %.2f, %.2f", pos_.x, pos_.y, pos_.z);
}

/// <summary>
/// �ʒu���̎擾
/// </summary>
/// <returns>�ʒu���</returns>
VECTOR Player::GetPos()
{
	return pos_;
}

void Player::SetCamera(shared_ptr<Camera> pCamera)
{
	pCamera_ = pCamera;
}