#include "Player.h"
#include "Camera.h"
#include "Util/InputState.h"
#include "Util/Model.h"
#include "Util//Effekseer3DEffectManager.h"
#include <string>

namespace
{
	// �v���C���[���f���̃t�@�C���̃p�X
	std::string model_file_path = "Data/Model/MV1/Player.mv1";

	// �v���C���[�̈ړ���
	constexpr VECTOR player_vec_up = { 0, 0, -1 };
	constexpr VECTOR player_vec_down = { 0, 0, 1 };
	constexpr VECTOR player_vec_right = { 1, 0, 0 };
	constexpr VECTOR player_vec_left = { -1, 0, 0 };

	// ���f���̊g�嗦
	constexpr float model_scale = 0.5f;

	// �v���C���[�̒ʏ�ړ����x
	constexpr float move_normal_speed = 1.5f;

	// �v���C���[�̃u�[�X�g���ړ����x
	constexpr float move_boost_speed = 3.0f;

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
	constexpr float model_collision_radius = 30.0f;

	// �������S����̍���
	constexpr float death_judg_height = -1000.0f;

	// ���G���Ԃ̃t���[����
	constexpr int ultimate_frames = 120;

	// HP
	constexpr int hp = 5;
}

//  �R���X�g���N�^
Player::Player(UnityGameObject data) :
	pos_(data.pos),
	rot_(data.rot),
	moveVec_(VGet(0.0f, 0.0f, 0.0f)),
	effectRot_({0, 0, 0}),
	hp_(hp),
	ultimateTimer_(0),
	isInput_(false),
	moveSpeed_(move_normal_speed),
	energyGauge_(energy_gauge_total_amount),
	slowRate_(1.0f),
	boostEffectScale_(50.0f), 
	playerDeadEffectHandle_(-1),
	isPlayGameOverEffect_(false),
	isBoost_(false),
	isSlow_(false),
	isReverseMoveVec_(false)
{
	// �v���C���[���f���̃C���X�^���X�̍쐬
	pModel_ = std::make_shared<Model>(model_file_path.c_str());

	// ���f���̊g�嗦�̐ݒ�
//	pModel_->SetScale(data.scale);
	pModel_->SetScale(VGet(model_scale, model_scale, model_scale));

	// ��]���̐ݒ�
	pModel_->SetRot(rot_);

	// �ʒu���̐ݒ�
	pModel_->SetPos(pos_);

	// �A�j���[�V�����Ɠ����蔻��̍X�V
	pModel_->Update();
}

//  �f�X�g���N�^
Player::~Player()
{
	// �����Ȃ�
}

// �X�V
void Player::Update()
{
	// ���X�e�B�b�N�̓��͏��̎擾
	int up = InputState::IsPadStick(PadLR::LEFT, PadStickInputType::UP);
	int down = InputState::IsPadStick(PadLR::LEFT, PadStickInputType::DOWN);
	int left = InputState::IsPadStick(PadLR::LEFT, PadStickInputType::LEFT);
	int right = InputState::IsPadStick(PadLR::LEFT, PadStickInputType::RIGHT);

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

	// �X���[���[�V�����̏���
	SlowProcess();

	// �u�[�X�g�̏���
	BoostProcess();
	
	// �G�l���M�[�̏���
	EnergyProcess();

	// �ړ����̏�����
	isInput_ = false;
	moveVec_ = VGet(0, 0, 0);
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
		moveVec_ = VAdd(moveVecZ, moveVecX);

		// �v���C���[�̃X�s�[�h���|����
		moveVec_ = VScale(moveVec_, moveSpeed_);

		// �X���[���[�V�����̃��[�g���|����
		moveVec_ = VScale(moveVec_, slowRate_);

		// �쐬�����ړ��x�N�g���ō��W�̈ړ�
		pos_ = VAdd(pos_, moveVec_);
	}
	else
	{
		// �����ĂȂ��ꍇ���X���[���[�V��������Ȃ��ꍇ�͒ʏ�X�s�[�h�ɐ؂�ւ���
		if (!isSlow_)
		{
			isBoost_ = false;
		}
	}

	// ���G���Ԃ̃^�C�}�[�̍X�V
	// 0�ȉ��ɂ͂Ȃ�Ȃ�
	ultimateTimer_ = (std::max)(--ultimateTimer_, 0);

	// �ړ��x�N�g���̑傫������v���C���[�̌X������Z�o
	rot_ = VGet(moveVec_.z * DX_PI_F / 180.0f / slowRate_, 0.0f, -moveVec_.x * DX_PI_F / 180.0f / slowRate_);

	// �ʒu���W�̐ݒ�
	pModel_->SetPos(pos_);

	// �����Ă�������̐ݒ�
	pModel_->SetRot(rot_);

	// �A�j���[�V������i�߂�
	pModel_->Update();
}

// ��Ƃ̏Փˎ��̍X�V
bool Player::CollisionRockUpdate()
{
	// �u�[�X�g���̃G�t�F�N�g�̍Đ��̃X�g�b�v
	Effekseer3DEffectManager::GetInstance().StopEffect(boostEffectHandle_);
	
	// �ړ��x�N�g���𔽓]���Ă��Ȃ������甽�]
	// ���ɔ��]���Ă����甽�]���Ȃ�
	if (!isReverseMoveVec_)
	{
		isReverseMoveVec_ = true;
		moveVec_ = VScale(moveVec_, -1.0f);
	}

	// �x�N�g�������X�ɏ���������
	moveVec_ = VScale(moveVec_, 0.96f);

	// �쐬�����ړ��x�N�g���ō��W�̈ړ�
	pos_ = VAdd(pos_, moveVec_);

	// �x�N�g��������̑傫�������������Ȃ�����Q�[���I�[�o�[�G�t�F�N�g�Đ�
	if(VSize(moveVec_) <= 1.0f)
	{
		// �_���[�W����
		OnDamage();

		if (hp_ <= 0)
		{
			// �Q�[���I�[�o�[�G�t�F�N�g���Đ����ĂȂ�������Đ�
			// ���ɍĐ����Ă�����Đ����Ȃ�
			if (!isPlayGameOverEffect_)
			{
				isPlayGameOverEffect_ = true;
				float scale = 50.0f, speed = 0.5f;
				VECTOR rot{ 0.0f, 0.0f, 0.0f };
				Effekseer3DEffectManager::GetInstance().PlayEffect(&playerDeadEffectHandle_, "explosion2", PlayType::NORMAL, &pos_, &scale, &speed, &rot);
			}
			// �G�t�F�N�g���Đ����I������true��Ԃ�
			if (!Effekseer3DEffectManager::GetInstance().IsPlayingEffect(playerDeadEffectHandle_) && isPlayGameOverEffect_)
			{
				// �����I��
				return true;
			}
		}
		else
		{
			// ������
			isReverseMoveVec_ = false;

			// �����I��
			return true;
		}
	}
	
	// �ʒu���W�̐ݒ�
	pModel_->SetPos(pos_);

	// �����Ă�������̐ݒ�
	pModel_->SetRot(VGet(VSize(moveVec_) / 5.0f, 0.0f, VSize(moveVec_) / 10.0f));

	// �A�j���[�V�����Ɠ����蔻��̍X�V
	pModel_->Update();

	// �������r���Ȃ̂�false��Ԃ�
	return false;
}

// �u�[�X�g�̏���
void Player::BoostProcess()
{
	// �u�[�X�g�؂�ւ�
	if (InputState::IsTriggered(InputType::BOOST))
	{
		// ��u�[�X�g�����G�l���M�[�Q�[�W�̎c�ʂ��������ꍇ�u�[�X�g�Ɉڍs
		if (!isBoost_ && energyGauge_ > 0)
		{
			isBoost_ = true;

			// �u�[�X�g���̃G�t�F�N�g���Đ�
			Effekseer3DEffectManager::GetInstance().PlayEffect(&boostEffectHandle_, "starFire", PlayType::LOOP_AND_FOLLOW, &pos_, &boostEffectScale_, &slowRate_, &effectRot_);
		}
		// �u�[�X�g���̏ꍇ�͒ʏ푬�x�Ɉڍs
		else
		{
			isBoost_ = false;
		}
	}
	// �u�[�X�g��
	if (isBoost_)
	{
		// �X���[���[�V�������̓u�[�X�g�̃G�t�F�N�g�̑傫��������������
		// �ʏ펞�͒ʏ�̃G�t�F�N�g�̑傫��
		(isSlow_) ?
			(boostEffectScale_ = 25.0f) :
			(boostEffectScale_ = 40.0f);

		// ���X�ɉ���
		moveSpeed_ += 1.0f;

		// ����̃X�s�[�h���傫���Ȃ�Ȃ�
		if (moveSpeed_ > move_boost_speed)
		{
			moveSpeed_ = move_boost_speed;
		}
	}
	// ��u�[�X�g��
	else
	{
		// �u�[�X�g���̃G�t�F�N�g�̍Đ����X�g�b�v
		Effekseer3DEffectManager::GetInstance().StopEffect(boostEffectHandle_);
			
		// ���X�Ɍ���
		moveSpeed_ -= 1.0f;

		// ����̃X�s�[�h��菬�����Ȃ�Ȃ�
		if (moveSpeed_ < move_normal_speed)
		{
			moveSpeed_ = move_normal_speed;
		}
	}
}

// �X���[���[�V�����̏���
void Player::SlowProcess()
{
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
		// ���X�ɃX���[�ɂ��Ă���
		slowRate_ -= 0.1f;

		// ����̒l���͏������Ȃ�Ȃ�
		if (slowRate_ < slow_rate)
		{
			slowRate_ = slow_rate;
		}
	}
	else
	{
		// ���X�ɑ������Ă���
		slowRate_ += 0.1f;

		// ����̒l���͑傫���Ȃ�Ȃ�
		if (slowRate_ >= 1.0f)
		{
			slowRate_ = 1.0f;
		}
	}
}

// �G�l���M�[����
void Player::EnergyProcess()
{
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
		energyGauge_ += energy_recovery_amount;

		// �G�l���M�[����ɒB������񕜂��~�߂�
		if (energyGauge_ > energy_gauge_total_amount)
		{
			energyGauge_ = energy_gauge_total_amount;
		}
	}
}

// �`��
void Player::Draw()
{
#ifdef _DEBUG
	DrawFormatString(10, 80, 0xffffff, "playerPos = %.2f, %.2f, %.2f", pos_.x, pos_.y, pos_.z);
	DrawFormatString(10, 105, 0xffffff, "energyGauge = %.2f", energyGauge_);
	DrawFormatString(10, 135, 0xffffff, "hp = %d", hp_);
#endif

	// ���G���Ԃ̓_��
	if (IsUltimate())
	{
		if ((ultimateTimer_ / 5) % 2 == 0)
		{
			return;
		}
	}

	// �v���C���[���f���̕`��
	if (!isPlayGameOverEffect_)
	{
		pModel_->Draw();
#ifdef _DEBUG
		DrawSphere3D(pos_, model_collision_radius, 8, 0xff0000, 0xff0000, false);
#endif 
	}
}

// �v���C���[�̗�������
void Player::Fall(float fallSpeed)
{
	// ����
	pos_.y -= (fallSpeed * slowRate_);
}

// �v���C���[�̃��X�|�[������
void Player::Respawn(VECTOR restartPos)
{
	// Y���̒l��傫������
	// ���̂܂܂��Ɠ��ɂ߂荞�ނ���
	restartPos = VGet(restartPos.x, restartPos.y + 200.0f, restartPos.z);

	// ���X�|�[��
	pos_ = restartPos;
}

// �_���[�W����
void Player::OnDamage()
{
	// ���G���Ԓ��̓_���[�W���󂯂Ȃ�
	if (IsUltimate()) return;

	// HP�����炷
	hp_--;

	// ���G���Ԃ̐ݒ�
	ultimateTimer_ = ultimate_frames;
}

// �v���C���[�̍������������S����̍�����菬�����Ȃ�����
bool Player::IsDeathJudgHeight() const
{
	return (pos_.y < death_judg_height) ? true : false;
}

// �v���C���[�����G���Ԓ���
bool Player::IsUltimate() const
{
	return (ultimateTimer_ > 0) ? true : false;
}

bool Player::IsLive() const
{
	return (hp_ > 0 ) ? true : false;
}

// �ʒu���̎擾
VECTOR Player::GetPos() const
{
	return pos_;
}

// �u�[�X�g��Ԃ��̎擾
bool Player::GetIsBoost() const
{
	return isBoost_;
}

// �X���[���[�V�����̃��[�g�̎擾
float Player::GetSlowRate() const
{
	return slowRate_;
}

// �v���C���[�̓����蔻��̔��a�̎擾
float Player::GetCollsionRadius() const
{
	return model_collision_radius;
}

// �v���C���[���f���̃n���h���̎擾
int Player::GetModelHandle() const
{
	return pModel_->GetModelHandle();
}

// �J�����N���X�̃|�C���^�̐ݒ�
void Player::SetCameraPointer(std::shared_ptr<Camera> pCamera)
{
	pCamera_ = pCamera;
}

float Player::GetCameraFar() const
{
	return pCamera_->GetCameraFar();
}
