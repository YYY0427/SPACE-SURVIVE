#include "BossEnemy.h"
#include "../Util/Model.h"
#include "../Util/Timer.h"
#include "../UI/HpBar.h"
#include "../Laser/LazerManager.h"
#include "../Player.h"
#include "../Util/Effekseer3DEffectManager.h"
#include "../Util/Debug.h";
#include "../MathUtil.h"
#include <random>
#include <algorithm>

namespace
{
	// HP
	constexpr int hp_bar_side_space = 300;
	constexpr int hp_bar_start_y = 50;
	constexpr int hp_bar_height = 30;
	constexpr float aim_hp_speed = 0.09f;
	constexpr float max_hp = 100.0f;

	// �L���[�u���[�U�[�̔��ˈʒu�t���[��
	constexpr int cube_laser_fire_frame_1 = 4;
	constexpr int cube_laser_fire_frame_2 = 5;

	// �ʏ탌�[�U�[�̔��ˈʒu�t���[��
	constexpr int normal_laser_fire_frame = 2;

	// �����ʒu
	constexpr VECTOR start_init_pos = { 0, 300, 1100 };
	constexpr VECTOR goal_init_pos = { 0, 300, 800 };

	// �g�嗦
	constexpr VECTOR model_scale = { 1, 1, 1 };

	// ��]��
	constexpr VECTOR rot = { 20 * DX_PI_F / 180.0f, DX_PI_F, 0 };

	constexpr VECTOR init_model_direction = { 0, 0, -1 };

	// �A�j���[�V�����ԍ�
	constexpr int idle_anim_no = 0;					// �ҋ@
	constexpr int normal_laser_fire_anim_no = 1;	// �ʏ탌�[�U�[�̔��ˎ�
	constexpr int cube_laser_fire_anim_no = 3;		// �L���[�u���[�U�[�̔��ˎ�

	// ���[�U�[�̔��ˊԊu
	constexpr int cube_laser_interval_frame = 60 * 2;
	constexpr int normal_laser_interval_frame = 60 * 10;

	// ���[�U�[�̑��x
	constexpr float cube_laser_speed = 5.0f;	

	// �����蔻��̔��a
	constexpr float collision_radius = 250.0f;

	// �ړ�����n�_
	constexpr VECTOR move_pos[] =
	{
		goal_init_pos,
		{ 930, 300, 800 },
		{ 930, -300, 800 },
		{ -930, 300, 800 },
		{ -930, -300, 800 },
	};

	// �ړ����鑬�x
	constexpr float move_speed = 10.0f;

	// �ړ��̌덷�͈�
	// �ڕW�̒n�_�ɓ��B�������m���߂鎞�A�덷���������邽�ߌ덷�͈̔͂�ݒ肷��
	constexpr float move_error_range = 10.0f;
}

BossEnemy::BossEnemy(int modelHandle, std::shared_ptr<Player> pPlayer, std::shared_ptr<LazerManager> pLazerManager) :
	updateFunc_(&BossEnemy::EntryUpdate),
	entryEffectH_(-1),
	isNormalLaser_(false),
	isGoal_(false),
	isMoveUpdateInit_(false),
	movePoint_(0)
{
	for (auto& point : move_pos)
	{
		movePointTable_.push_back(point);
	}

	pPlayer_ = pPlayer;
	pLazerManager_ = pLazerManager;
	normalLaserFireIntervalTimer_ = normal_laser_interval_frame;
	cubeLaserFireIntervalTimer_ = cube_laser_interval_frame;
	nextUpdateIdleTimer_ = 180;
	pos_ = start_init_pos;
	rot_ = rot;
	cubeLaserSpeed_ = cube_laser_speed;
	hp_ = max_hp;
	opacity_ = 0.0f;
	moveVec_ = { 0, 0, 0 };
	collisionRadius_ = collision_radius;

	pModel_ = std::make_unique<Model>(modelHandle);
	pHpBar_ = std::make_unique<HpBar>(max_hp);

	pModel_->SetOpacity(opacity_);
	pModel_->SetScale(model_scale);
	pModel_->SetRot(rot_);
	pModel_->SetPos(pos_);
	pModel_->Update();
}

BossEnemy::~BossEnemy()
{
}

void BossEnemy::Update()
{
	(this->*updateFunc_)();

	// �ʏ탌�[�U�[�̔��ˈʒu�̃t���[�����W�̎擾
	normalLaserFirePos_ = MV1GetFramePosition(pModel_->GetModelHandle(), normal_laser_fire_frame);

	// �ʏ탌�[�U���v���C���[�Ɍ������x�N�g���̍쐬
	toTargetVec_ = VSub(pPlayer_->GetPos(), normalLaserFirePos_);
	toTargetVec_ = VNorm(toTargetVec_);

	// �o�ꒆ�̏ꍇ�̓v���C���[�̕����������Ȃ�
	if (updateFunc_ != &BossEnemy::EntryUpdate)
	{
		// �x�N�g�������̉�]�s����p�x�ɕϊ�
		bool isGimbalLock = false;
		MATRIX rotMtx = MGetRotVec2(init_model_direction, toTargetVec_);
		rot_ = MathUtil::ToEulerAngles(rotMtx, isGimbalLock);
		rot_.y += DX_PI_F;
		rot_.x *= -1;
		rot_.z *= -1;
	}

	// �ʒu�̍X�V
	pos_ = VAdd(pos_, moveVec_);

	// HP�o�[�̍X�V
	pHpBar_->Update(aim_hp_speed);

	// ���f���ݒ�
	pModel_->SetOpacity(opacity_);	// �s�����x�̐ݒ�
	pModel_->SetRot(rot_);			// ��]���̐ݒ�
	pModel_->SetPos(pos_);			// �ʒu���̐ݒ�
	pModel_->Update();				// ���f���̓����蔻��A�A�j���[�V�����̍X�V
}

void BossEnemy::EntryUpdate()
{
	// �s�����x�����X�ɑ傫������
	opacity_ += 0.005f;

	// �ڕW�̒n�_�܂ŏ��X�ɋ߂Â�
	if (pos_.z >= goal_init_pos.z)
	{
		pos_.z -= 1.5f;
	}
	else
	{
		// �ڕW�̒n�_�܂ł̓��B������A�j���[�V�����J�n
		pModel_->SetAnimation(idle_anim_no, true, false);
	}

	// HP�o�[�̉��o���I������ꍇ
	if (pHpBar_->IsEndFirstDirection() && pos_.z <= goal_init_pos.z)
	{
		updateFunc_ = &BossEnemy::IdleUpdate;
	}
}

void BossEnemy::IdleUpdate()
{
	pModel_->ChangeAnimation(idle_anim_no, true, false, 8);

	nextUpdateIdleTimer_.Update(1);
	if (nextUpdateIdleTimer_.IsTimeOut())
	{
		updateFunc_ = &BossEnemy::MoveUpdate;

		nextUpdateIdleTimer_.Reset();
	}
}

void BossEnemy::CubeLaserAttackUpdate()
{
	// ���Ԋu�ŃL���[�u���[�U�[�̔���
//	cubeLaserFireIntervalTimer_.Update(1);
//	if (cubeLaserFireIntervalTimer_.IsTimeOut())
	{
		pModel_->ChangeAnimation(cube_laser_fire_anim_no, true, false, 8, 0.5f);

	//	if (!pModel_->IsAnimEnd())
		{
			// ���[�U�[�̔��ˈʒu�̃t���[�����W�̎擾
			VECTOR firePos = MV1GetFramePosition(pModel_->GetModelHandle(), cube_laser_fire_frame_1);

			// �v���C���[�Ɍ������x�N�g���̍쐬
			VECTOR vec = VSub(pPlayer_->GetPos(), firePos);
			vec = VNorm(vec);
			vec = VScale(vec, cubeLaserSpeed_);

			// ���[�U�[�̔���
			pLazerManager_->Create(LazerType::CUBE, &firePos, &vec, {});

			// �^�C�}�[�̏�����
			cubeLaserFireIntervalTimer_.Reset();
		}
	}
}

void BossEnemy::NormalLaserAttackUpdate()
{
	// �ʏ탌�[�U�[���˗p�̃A�j���[�V�����ɕύX
	pModel_->ChangeAnimation(normal_laser_fire_anim_no, false, false, 8);

	// �ʏ탌�[�U�[�̔���
	if (!isNormalLaser_)
	{
		pLazerManager_->Create(LazerType::NORMAL, &normalLaserFirePos_, &toTargetVec_, &moveVec_);
		isNormalLaser_ = true;
	}

	// �A�j���[�V�������I�������ꍇ
	if (pModel_->IsAnimEnd())
	{
		// ������
		isNormalLaser_ = false;

		// update��ύX
		updateFunc_ = &BossEnemy::IdleUpdate;
	}
}

void BossEnemy::MoveUpdate()
{
	if (!isMoveUpdateInit_)
	{
		// �n�_�̈ړ������̓���ւ�(�z��̒��g���V���b�t��)
		std::random_device seed;
		std::mt19937 engine(seed());
		std::shuffle(movePointTable_.begin(), movePointTable_.end(), engine);

		// ��ԍŏ��̖ڕW�n�_�̐ݒ�
		goalPos_ = movePointTable_.front();

		// ������
		movePoint_ = 0;

		// �x�N�g���̍쐬
		VECTOR vec = VSub(goalPos_, pos_);
		vec = VNorm(vec);
		vec = VScale(vec, move_speed);
		moveVec_ = vec;
		
		// ���̏����ɓ���Ȃ��悤�t���O����
		isMoveUpdateInit_ = true;
	}
	else
	{
		// �ڕW�̒n�_�ɓ��B����
		if (isGoal_)
		{
			// ������
			isGoal_ = false;

			// �ڕW�n�_�̍X�V
			movePoint_++;

			// ���[�U�[����
			CubeLaserAttackUpdate();

			// �S�Ă̒n�_���ړ�������
			if (static_cast<int>(movePointTable_.size()) <= movePoint_)
			{
				// �����ʒu�ɖ߂�����
				if (static_cast<int>(movePointTable_.size()) + 1 <= movePoint_)
				{
					// ������
					moveVec_ = { 0, 0, 0 };
					isMoveUpdateInit_ = false;

					// update�̕ύX
					updateFunc_ = &BossEnemy::IdleUpdate;
					return;
				}

				// �����ʒu��ڕW�n�_�ɐݒ�
				goalPos_ = goal_init_pos;
			}
			else
			{
				// ���Ԃɒn�_�̎擾
				goalPos_ = movePointTable_.at(movePoint_);
			}
			// �x�N�g���̍쐬
			VECTOR vec = VSub(goalPos_, pos_);
			vec = VNorm(vec);
			vec = VScale(vec, move_speed);
			moveVec_ = vec;
		}
		// �ڕW�̒n�_�ɓ��B���ĂȂ�
		else
		{
			// �ڕW�̒n�_�ɓ��B������t���O�𗧂Ă�
			if (pos_.x <= goalPos_.x + move_error_range && goalPos_.x - move_error_range <= pos_.x &&
				pos_.y <= goalPos_.y + move_error_range && goalPos_.y - move_error_range <= pos_.y &&
				pos_.z <= goalPos_.z + move_error_range && goalPos_.z - move_error_range <= pos_.z)
			{
				isGoal_ = true;
			}
		}
	}
	Debug::Log("movePoint", movePoint_);
}

void BossEnemy::DiedUpdate()
{
	/*Effekseer3DEffectManager::GetInstance().PlayEffect(
		onDamageEffectHandle_,
		EffectID::enemy_died,
		pos,
		200.0f,
		0.5f);*/
}

void BossEnemy::Draw()
{
	pModel_->Draw();

	DrawSphere3D(pos_, collisionRadius_, 8, 0xff0000, 0xff0000, false);

	pHpBar_->Draw(hp_bar_side_space, hp_bar_start_y, hp_bar_height);

	Debug::Log("bossEnemy", pos_);
}

void BossEnemy::OnDamage(int damage, VECTOR pos)
{
	hp_ -= damage;

	pHpBar_->OnDamage(hp_);

	if (hp_ <= 0)
	{
		updateFunc_ = &BossEnemy::DiedUpdate;
	}
}
