#include "Player.h"
#include "Camera.h"
#include "Util/InputState.h"
#include "Util/Model.h"
#include <string>

namespace
{
	// �v���C���[���f���̃t�@�C���̃p�X
	string model_file_path = "Data/Model/UFO.mv1";

	// �v���C���[�̈ړ���
	constexpr VECTOR player_vec_up{ 0, 0, -1 };
	constexpr VECTOR player_vec_down{ 0, 0, 1 };
	constexpr VECTOR player_vec_right{ 1, 0, 0 };
	constexpr VECTOR player_vec_left{ -1, 0, 0 };

	// �v���C���[�̒ʏ�ړ����x
	constexpr float move_normal_speed = 1.0f;

	// �v���C���[�̃u�[�X�g���ړ����x
	constexpr float move_boost_speed = 3.0f;

	// �v���C���[�̏㏸�A���~�X�s�[�h
	constexpr float move_y_speed = 5.0f;

	// �X���[���[�V�����̃��[�g
	constexpr float slow_rate = 0.1f;

	// �u�[�X�g���̃G�l���M�[�����
	constexpr float boost_energy_cost = 10.0f;

	// �G�l���M�[�Q�[�W
	constexpr float energy_gauge_total_amount = 1000.0f;

	// �G�l���M�[�̉񕜗�
	constexpr float energy_recovery_amount = 10.0f;
}

/// <summary>
///  �R���X�g���N�^
/// </summary>
Player::Player() :
	pos_(VGet(0.0f, 0.0f, 0.0f)),
	isInput_(false),
	moveSpeed_(move_normal_speed),
	energyGauge_(energy_gauge_total_amount),
	slowRate_(1.0f)
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
	// ���X�e�B�b�N�̓��͏��̎擾
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

	if (InputState::IsTriggered(InputType::SLOW))
	{
		if (slowRate_ == slow_rate)
		{
			slowRate_ = 1.0f;
		}
		else
		{
			slowRate_ = slow_rate;
		}
	}

	// �u�[�X�g�؂�ւ�
	if (InputState::IsTriggered(InputType::BOOST))
	{
		// ��u�[�X�g�����G�l���M�[�Q�[�W�̎c�ʂ��������ꍇ�u�[�X�g�Ɉڍs
		if (moveSpeed_ == move_normal_speed && energyGauge_ > 0)
		{
			moveSpeed_ = move_boost_speed;
		}
		// �u�[�X�g���̏ꍇ�͒ʏ푬�x�Ɉڍs
		else
		{
			moveSpeed_ = move_normal_speed;
		}
	}
	// �u�[�X�g���̓G�l���M�[�����葱����
	// �G�l���M�[���Ȃ��Ȃ�����ʏ푬�x�Ɉڍs
	if (moveSpeed_ == move_boost_speed)
	{
		energyGauge_ -= (boost_energy_cost * slowRate_);
		if (energyGauge_ <= 0)
		{
			moveSpeed_ = move_normal_speed;
		}
	}
	// ��u�[�X�g���Ȃ�G�l���M�[�͉�
	else
	{
		energyGauge_ += (energy_recovery_amount * slowRate_);
		if (energyGauge_ > energy_gauge_total_amount)
		{
			energyGauge_ = energy_gauge_total_amount;
		}
	}

	// �ړ����̏�����
	isInput_ = false;
	VECTOR moveVec = VGet(0, 0, 0);
	VECTOR moveVecX = VGet(0, 0, 0);
	VECTOR moveVecZ = VGet(0, 0, 0);
#if false
	if (up > 0)
	{
		moveVecZ = moveUp;
		isMove_ = true;
	}
	if (left > 0)
	{
		moveVecX = moveLeft;
		isMove_ = true;
	}
	if (down > 0)
	{
    	moveVecZ = moveDown;
		isMove_ = true;
	}
	if (right > 0)
	{
		moveVecX = moveRight;
		isMove_ = true;
	}
#else 
	// �X�e�B�b�N�����͂���Ă�����ړ��x�N�g���ɃX�e�B�b�N���X���Ă�������̃x�N�g������
	// �X�e�B�b�N�̌X�����킢�ɂ���ăx�N�g�����傫���Ȃ�
	if (up > 0)
	{
		moveVecZ = VScale(moveUp, up);
		isInput_ = true;
	}
	if (left > 0)
	{
		moveVecX = VScale(moveLeft, left);
		isInput_ = true;
	}
	if (down > 0)
	{
		moveVecZ = VScale(moveDown, down);
		isInput_ = true;
	}
	if (right > 0)
	{
		moveVecX = VScale(moveRight, right);
		isInput_ = true;
	}
#endif
	// �X�e�B�b�N�����͂���Ă���ꍇ�݈̂ړ�
	if (isInput_)
	{
		// x������z�����̃x�N�g���𑫂��Ĉړ��x�N�g�����쐬����
		moveVec = VAdd(moveVecZ, moveVecX);

		// ���K��
	//	moveVec = VNorm(moveVec);

		// ���K�������x�N�g���Ƀv���C���[�̑��x��������
		moveVec = VScale(moveVec, moveSpeed_);

		moveVec = VScale(moveVec, slowRate_);

		// �쐬�����ړ��x�N�g���ō��W�̈ړ�
		pos_ = VAdd(pos_, moveVec);
	}
	// Y���̈ړ�
	/*if (InputState::IsPressed(InputType::RIZE))
	{
		pos_.y += 3.0f;
	}*/
	if (InputState::IsXInputTrigger(XInputType::RIGHT))
	{
		pos_.y += move_y_speed * slowRate_;
	}
	if (InputState::IsXInputTrigger(XInputType::LEFT))
	{
		pos_.y -= move_y_speed * slowRate_;
	}

	// �ʒu���W�̐ݒ�
	pModel_->SetPos(pos_);

	// �����Ă�������̐ݒ�
	pModel_->SetRot(VGet(moveVec.z * DX_PI_F / 180.0f, 0.0f, -moveVec.x * DX_PI_F / 180.0f));

	// �A�j���[�V������i�߂�
	pModel_->Update();
}

/// <summary>
/// �`��
/// </summary>
void Player::Draw()
{
	// �v���C���[���f���̕`��
	pModel_->Draw();
#ifdef _DEBUG
	DrawFormatString(10, 80, 0xffffff, "playerPos = %.2f, %.2f, %.2f", pos_.x, pos_.y, pos_.z);
	DrawFormatString(10, 105, 0xffffff, "energyGauge = %.2f", energyGauge_);
#endif
}

/// <summary>
/// �ʒu���̎擾
/// </summary>
/// <returns>�ʒu���</returns>
VECTOR Player::GetPos()
{
	return pos_;
}

/// <summary>
/// �u�[�X�g��Ԃ��̎擾
/// </summary>
/// <returns>true : �u�[�X�g��ԁAfalse : �ʏ���</returns>
bool Player::GetIsBoost()
{
	if (moveSpeed_ == move_boost_speed)
	{
		return true;
	}
	return false;
}

/// <summary>
/// �X���[���[�V�����̃��[�g�̎擾
/// </summary>
/// <returns>�X���[���[�V�����̃��[�g</returns>
float Player::GetSlowRate()
{
	return slowRate_;
}

/// <summary>
/// �J�����N���X�̃|�C���^�̃Z�b�^�[
/// </summary>
/// <param name="pCamera">�J�����N���X�̃|�C���^</param>
void Player::SetCamera(shared_ptr<Camera> pCamera)
{
	pCamera_ = pCamera;
}