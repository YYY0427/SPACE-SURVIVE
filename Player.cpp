#include "Player.h"
#include "Camera.h"
#include "Util/InputState.h"
#include "Util/Model.h"
#include "Util//Effekseer3DEffectManager.h"
#include <string>

namespace
{
	// �v���C���[���f���̃t�@�C���̃p�X
	std::string model_file_path = "Data/Model/UFO.mv1";

	// �v���C���[�̈ړ���
	constexpr VECTOR player_vec_up{ 0, 0, -1 };
	constexpr VECTOR player_vec_down{ 0, 0, 1 };
	constexpr VECTOR player_vec_right{ 1, 0, 0 };
	constexpr VECTOR player_vec_left{ -1, 0, 0 };

	// �v���C���[�̒ʏ�ړ����x
	constexpr float move_normal_speed = 3.0f;

	// �v���C���[�̃u�[�X�g���ړ����x
	constexpr float move_boost_speed = 6.0f;

	// �v���C���[�̏㏸�A���~�X�s�[�h
	constexpr float move_y_speed = 5.0f;

	// �X���[���[�V�����̃��[�g
	constexpr float slow_rate = 0.1f;

	// �X���[���[�V�������̃G�l���M�[�����
	constexpr float slow_energy_cost = 3.0f;

	// �u�[�X�g���̃G�l���M�[�����
	constexpr float boost_energy_cost = 1.0f;

	// �G�l���M�[�Q�[�W
	constexpr float energy_gauge_total_amount = 1000.0f;

	// �G�l���M�[�̉񕜗�
	constexpr float energy_recovery_amount = 10.0f;

	// �v���C���[�̓����蔻��̔��a
	constexpr float model_collision_radius = 60.0f;
}

/// <summary>
///  �R���X�g���N�^
/// </summary>
Player::Player() :
	pos_(VGet(0.0f, 2000.0f, 0.0f)),
	isInput_(false),
	moveSpeed_(move_normal_speed),
	energyGauge_(energy_gauge_total_amount),
	slowRate_(1.0f),
	isEffectFirst_(false),
	isBoost_(false),
	isSlow_(false)
{
	pModel_ = std::make_shared<Model>(model_file_path.c_str());
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

	// �J�����������Ă����������x�N�g���ϊ����Ĉړ����쐬
#if true
	VECTOR moveForward = VScale(VNorm(VSub(pCamera_->GetTarget(), pCamera_->GetPos())), -1);
	VECTOR moveBack = VScale(moveForward, -1);
#else 
	VECTOR moveForward = VTransform(player_vec_up, MGetRotY(pCamera_->GetCameraYaw()));
	VECTOR moveBack = VTransform(player_vec_down, MGetRotY(pCamera_->GetCameraYaw()));
#endif
	VECTOR moveRight = VTransform(player_vec_right, MGetRotY(pCamera_->GetCameraYaw()));
	VECTOR moveLeft = VTransform(player_vec_left, MGetRotY(pCamera_->GetCameraYaw()));

	// �X���[���[�V�����؂�ւ�
	if (InputState::IsTriggered(InputType::SLOW))
	{
		// �X���[���[�V��������Ȃ����G�l���M�[�Q�[�W�̎c�ʂ���������X���[���[�V�����Ɉڍs
		if (!isSlow_ && energyGauge_ > 0)
		{
			isSlow_ = true;
		}
		// �X���[���[�V�������͒ʏ�Ɉڍs
		else
		{
			isSlow_ = false;
		}
	}
	// �t���O�������Ă�����X���[���[�V�����ɐ؂�ւ�
	if (isSlow_)
	{
		slowRate_ = slow_rate;
	}
	else
	{
		slowRate_ = 1.0f;
	}

	// �u�[�X�g�؂�ւ�
	if (InputState::IsTriggered(InputType::BOOST))
	{
		// ��u�[�X�g�����X���[���[�V��������Ȃ����G�l���M�[�Q�[�W�̎c�ʂ��������ꍇ�u�[�X�g�Ɉڍs
		if (!isBoost_ && energyGauge_ > 0 && !isSlow_)
		{
			isBoost_ = true;
		}
		// �u�[�X�g���̏ꍇ�͒ʏ푬�x�Ɉڍs
		else
		{
			isBoost_ = false;
		}
	}
	// �u�[�X�g���͏��X�ɉ���
	if (isBoost_)
	{
		moveSpeed_ += 1;
		if (moveSpeed_ > move_boost_speed)
		{
			moveSpeed_ = move_boost_speed;
		}
	}
	// ��u�[�X�g���͏��X�Ɍ���
	else
	{
		moveSpeed_ -= 1;
		if (moveSpeed_ < move_normal_speed)
		{
			moveSpeed_ = move_normal_speed;
		}
	}
	// �u�[�X�g���̓G�l���M�[�����葱����
	// �X���[���[�V�������̓u�[�X�g���̃G�l���M�[�͌���Ȃ�
	if (isBoost_ && !isSlow_)
	{
		energyGauge_ -= boost_energy_cost;

		// �G�l���M�[���Ȃ��Ȃ�����ʏ푬�x�Ɉڍs
		if (energyGauge_ <= 0)
		{
			isBoost_ = false;
		}
	}
	// �X���[���[�V�������̓G�l���M�[�����葱����
	else if (isSlow_)
	{
		energyGauge_ -= slow_energy_cost;

		// �G�l���M�[���Ȃ��Ȃ�����ʏ�Ɉڍs
		if (energyGauge_ <= 0)
		{
			isSlow_ = false;
		}
	}
	// ��u�[�X�g������X���[���[�V�������̓G�l���M�[�͉�
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

	// �X�e�B�b�N�����͂���Ă�����ړ��x�N�g���ɃX�e�B�b�N���X���Ă�������̃x�N�g������
	// �X�e�B�b�N�̌X�����킢�ɂ���ăx�N�g�����傫���Ȃ�
	if (up > 0)
	{
		moveVecZ = VScale(moveForward, up);
		isInput_ = true;
	}
	if (left > 0)
	{
		moveVecX = VScale(moveLeft, left);
		isInput_ = true;
	}
	if (down > 0)
	{
		moveVecZ = VScale(moveBack, down);
		isInput_ = true;
	}
	if (right > 0)
	{
		moveVecX = VScale(moveRight, right);
		isInput_ = true;
	}

	// �X�e�B�b�N�����͂���Ă���ꍇ�݈̂ړ�
	if (isInput_)
	{
		// �v���C���[���猩��x������z�����̃x�N�g���𑫂��Ĉړ��x�N�g�����쐬����
		moveVec = VAdd(moveVecZ, moveVecX);

		// �v���C���[�̃X�s�[�h���|����
		moveVec = VScale(moveVec, moveSpeed_);

		// �X���[���[�V�����̃��[�g���|����
		moveVec = VScale(moveVec, slowRate_);

		// �쐬�����ړ��x�N�g���ō��W�̈ړ�
		pos_ = VAdd(pos_, moveVec);
	}
	else
	{
		// �����ĂȂ��ꍇ�ʏ�X�s�[�h�ɐ؂�ւ���
		isBoost_ = false;
	}

	// �ʒu���W�̐ݒ�
	pModel_->SetPos(pos_);

	if (isSlow_)
	{
		// �����Ă�������̐ݒ�
		pModel_->SetRot(VGet(moveVec.z * DX_PI_F / 180.0f / slowRate_, 0.0f, -moveVec.x * DX_PI_F / 180.0f / slowRate_));
	}
	else
	{
		// �����Ă�������̐ݒ�
		pModel_->SetRot(VGet(moveVec.z* DX_PI_F / 180.0f, 0.0f, -moveVec.x * DX_PI_F / 180.0f));
	}
	
	// �A�j���[�V������i�߂�
	pModel_->Update();
}

/// <summary>
/// �Q�[���I�[�o�[���̃v���C���[�̍X�V
/// </summary>
/// <returns>�G�t�F�N�g���Đ����I������</returns>
bool Player::GameOverUpdate()
{
	// ��񂾂��G�t�F�N�g���Đ�
	if (!isEffectFirst_)
	{
		isEffectFirst_ = true;
		Effekseer3DEffectManager::GetInstance().PlayEffect("explosion", pos_, VGet(50.0f, 50.0f, 50.0f), 1.0f);
	}
	// �G�t�F�N�g���Đ����I������true��Ԃ�
	if (!Effekseer3DEffectManager::GetInstance().IsPlayingEffect("explosion"))
	{
		return true;
	}
	return false;

	// �ʒu���W�̐ݒ�
	pModel_->SetPos(pos_);

	// �A�j���[�V�����Ɠ����蔻��̍X�V
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
	DrawSphere3D(pos_, model_collision_radius, 8, 0xff0000, 0xff0000, false);
	DrawFormatString(10, 80, 0x000000, "playerPos = %.2f, %.2f, %.2f", pos_.x, pos_.y, pos_.z);
	DrawFormatString(10, 105, 0x000000, "energyGauge = %.2f", energyGauge_);
#endif
}

/// <summary>
/// �ʒu���̎擾
/// </summary>
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
	return isBoost_;
}

/// <summary>
/// �X���[���[�V�����̃��[�g�̎擾
/// </summary>
float Player::GetSlowRate()
{
	return slowRate_;
}

/// <summary>
/// �v���C���[�̓����蔻��̔��a�̎擾
/// </summary>
float Player::GetCollsionRadius()
{
	return model_collision_radius;
}

/// <summary>
/// �J�����N���X�̃|�C���^��
/// </summary>
void Player::SetCameraPointer(std::shared_ptr<Camera> pCamera)
{
	pCamera_ = pCamera;
}