#include "BossEnemy.h"
#include "../Util/Model.h"
#include "../Util/Timer.h"
#include "../UI/HpBar.h"
#include "../Laser/LaserManager.h"
#include "../Player.h"
#include "../Util/Effekseer3DEffectManager.h"
#include "../Util/Debug.h";
#include "../Util/MathUtil.h"
#include "../Util/InputState.h"
#include "../Triangle.h"
#include "../Flash.h"
#include "../ScreenEffect.h"
#include <random>
#include <algorithm>

// TODO : �v���C���[���c�@����HP�ɂ���(�Q�[�����r���Œ��f���Ă��܂�����
// TODO : �G�̔��˂ł��郌�[�U�[���������傢�e���|�����悭����(���߂ăv���C����l�ł������������Ȃ���v���C�ł���悤�ɂ��邽��)
// TODO : �G�̃��[�U�[�𔽎˂���Ƃ��^����_���[�W�𐧌�����(������x�G�Ƀ_���[�W��^����ƁA�G���V�[���h�𒣂�)
// TODO : �L���[�u���[�U�[�̉��s�𕪂���₷�����邽�߂ɁA�v���C���[�̐��ʂɌ����Ȃ������蔻������A�����ɃL���[�u���[�U�[���Ԃ���ƃG�t�F�N�g�𔭐�������
// TODO : �F����ԂɃI�u�W�F�N�g��u���ĉ��s�𕪂���₷������
// TODO : �ʔ����ǉ��@�G���G�͓|���Ȃ��Ă��������G���G��|���Γ|���قǃo���A���傫���Ȃ�{�X�킪�y�ɂȂ�
// TODO : �G���[�U�[�ƃv���C���[�V�[���h�̓����蔻��̏C��(���󌋍\�������Ⴄ)

namespace
{
	// HP
	constexpr int hp_bar_side_space = 300;
	constexpr int hp_bar_start_y = 50;
	constexpr int hp_bar_height = 30;
	constexpr float aim_hp_speed = 0.4f;
	constexpr float max_hp = 100.0f;

	// �����ʒu
	constexpr VECTOR start_init_pos = { 0, 300, 1100 };
	constexpr VECTOR goal_init_pos = { 0, 300, 800 };

	// �ҋ@�X�e�[�g�̌p�����ԃt���[��
	constexpr int idle_state_continue_frame = 180;

	////// ���f�� //////

	// �g�嗦
	constexpr VECTOR model_scale = { 1, 1, 1 };

	// ��]��
	constexpr VECTOR rot = { 20 * DX_PI_F / 180.0f, DX_PI_F, 0 };

	// ���f���̌����Ă������
	constexpr VECTOR init_model_direction = { 0, 0, -1 };

	// �A�j���[�V�����ԍ�
	constexpr int idle_anim_no = 0;					// �ҋ@
	constexpr int normal_laser_fire_anim_no = 1;	// �ʏ탌�[�U�[�̔��ˎ�
	constexpr int cube_laser_fire_anim_no = 3;		// �L���[�u���[�U�[�̔��ˎ�

	// ���f���̃}�e���A���ԍ�
	constexpr int body_material_no = 0;
	constexpr int lense_material_no = 1;
	constexpr int eye_material_no = 2;
	constexpr int laser_material_no = 3;

	////// ���[�U�[ //////

	// �L���[�u���[�U�[�̔��ˈʒu�t���[��
	constexpr int cube_laser_fire_frame_1 = 4;
	constexpr int cube_laser_fire_frame_2 = 5;

	// �ʏ탌�[�U�[�̔��ˈʒu�t���[��
	constexpr int normal_laser_fire_frame = 2;

	// ���[�U�[�U�������t���[���s����
	constexpr int cube_laser_attack_continue_frame = 60 * 10;

	// ���[�U�[�̔��ˊԊu
	constexpr int cube_laser_interval_frame = 60 * 2;
	constexpr int normal_laser_interval_frame = 60 * 10;

	// ���[�U�[�̑��x
	constexpr float cube_laser_speed = 5.0f;	

	// �����蔻��̔��a
	constexpr float collision_radius = 250.0f;

	////// �ړ� //////

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

	////// ���S //////

	// ���S���̉��h��̑傫��
	constexpr float died_swing_width = 5.0f;

	// ���S���̉��h��̑���
	constexpr float died_swing_speed = 1.0f;

	// ���S���̉��o�̌p������
	constexpr int died_continue_frame = 60 * 5;
}

// �R���X�g���N�^
BossEnemy::BossEnemy(int modelHandle, std::shared_ptr<Player> pPlayer, std::shared_ptr<LaserManager> pLaserManager, std::shared_ptr<ScreenEffect> pScreenEffect) :
	isGoal_(false),
	isMoveEnd_(false),
	isDraw_(true),
	movePoint_(0),
	attackState_(0)
{
	// ������
	pPlayer_ = pPlayer;
	pLaserManager_ = pLaserManager;
	pScreenEffect_ = pScreenEffect;
	pos_ = start_init_pos;
	rot_ = rot;
	cubeLaserSpeed_ = cube_laser_speed;
	hp_ = max_hp;
	opacity_ = 0.0f;
	moveVec_ = { 0, 0, 0 };
	collisionRadius_ = collision_radius;
	for (auto& point : move_pos)
	{
		movePointTable_.push_back(point);
	}

	// �X�e�[�g�̏�����
	InitState();

	// �X�e�[�g�̐ݒ�
	stateMachine_.SetState(State::ENTRY);

	// �C���X�^���X����
	pModel_ = std::make_unique<Model>(modelHandle);
	pHpBar_ = std::make_unique<HpBar>(max_hp);

	// ���f���̐ݒ�
	pModel_->SetOpacity(opacity_);	// �s�����x
	pModel_->SetScale(model_scale);	// �g�嗦
	pModel_->SetRot(rot_);			// ��]��
	pModel_->SetPos(pos_);			// �ʒu
	pModel_->Update();				// �����蔻��A�A�j���[�V�����̍X�V
}

// �f�X�g���N�^
BossEnemy::~BossEnemy()
{
}

// �X�e�[�g�̍X�V
void BossEnemy::InitState()
{
	// �X�e�[�g�}�V��������
	stateMachine_.AddState(
		State::ENTRY,
		[this]() { this->EntarEntry(); },
		[this]() { this->UpdateEntry(); },
		[this]() { this->ExitEntry(); });
	stateMachine_.AddState(
		State::IDLE,
		[this]() { this->EntarIdle(); },
		[this]() { this->UpdateIdle(); },
		[this]() { this->ExitIdle(); });
	stateMachine_.AddState(
		State::MOVE,
		[this]() { this->EntarMove(); },
		[this]() { this->UpdateMove(); },
		[this]() { this->ExitMove(); });
	stateMachine_.AddState(
		State::DAMAGE,
		[this]() { this->EntarDamage(); },
		[this]() { this->UpdateDamage(); },
		[this]() { this->ExitDamage(); });
	stateMachine_.AddState(
		State::DEID,
		[this]() { this->EntarDied(); },
		[this]() { this->UpdateDied(); },
		[this]() { this->ExitDied(); });
	stateMachine_.AddState(
		State::STOP_ATTACK_CUBE_LASER,
		[this]() { this->EntarStopCubeLaserAttack(); },
		[this]() { this->UpdateStopCubeLaserAttack(); },
		[this]() { this->ExitStopCubeLaserAttack(); });
	stateMachine_.AddState(
		State::STOP_ATTACK_NORMAL_LASER,
		[this]() { this->EntarStopNormalLaserAttack(); },
		[this]() { this->UpdateStopNormalLaserAttack(); },
		[this]() { this->ExitStopNormalLaserAttack(); });
	stateMachine_.AddState(
		State::MOVE_ATTACK_CUBE_LASER,
		[this]() { this->EntarMoveCubeLaserAttack(); },
		[this]() { this->UpdateMoveCubeLaserAttack(); },
		[this]() { this->ExitMoveCubeLaserAttack(); });
	stateMachine_.AddState(
		State::MOVE_ATTACK_NORMAL_LASER,
		[this]() { this->EntarMoveNormalLaserAttack(); },
		[this]() { this->UpdateMoveNormalLaserAttack(); },
		[this]() { this->ExitMoveNormalLaserAttack(); });

	// �U���̃X�e�[�g��ۑ�
	attackStateTable_.push_back(State::MOVE_ATTACK_CUBE_LASER);
	attackStateTable_.push_back(State::MOVE_ATTACK_NORMAL_LASER);
	attackStateTable_.push_back(State::STOP_ATTACK_CUBE_LASER);
	attackStateTable_.push_back(State::STOP_ATTACK_NORMAL_LASER);
}

// �X�V
void BossEnemy::Update()
{
	if (InputState::IsTriggered(InputType::BOSS_DETH_DEBUG))
	{
		stateMachine_.SetState(State::DEID);	
	}

	// �X�e�[�g�}�V���̍X�V
	stateMachine_.Update();

	// �ʏ탌�[�U�[�̔��ˈʒu�̃t���[�����W�̎擾
	normalLaserFirePos_ = MV1GetFramePosition(pModel_->GetModelHandle(), normal_laser_fire_frame);

	// �ʏ탌�[�U���v���C���[�Ɍ������x�N�g���̍쐬
	toTargetVec_ = VSub(pPlayer_->GetPosLogTable().back(), normalLaserFirePos_);
	toTargetVec_ = VNorm(toTargetVec_);

	// �o�ꒆ�A���S���̏ꍇ�̓v���C���[�̕����������Ȃ�
	if(stateMachine_.GetCurrentState() != State::ENTRY &&
	   stateMachine_.GetCurrentState() != State::DEID)
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
	pModel_->RestoreAllMaterialDifColor();	// �f�B�t���[�Y�J���[�����ɖ߂�
	pModel_->SetOpacity(opacity_);	// �s�����x
	pModel_->SetRot(rot_);			// ��]��
	pModel_->SetPos(pos_);			// �ʒu
	pModel_->Update();				// ���f���̓����蔻��A�A�j���[�V�����̍X�V
}

void BossEnemy::GameOverUpdate()
{
	pModel_->ChangeAnimation(idle_anim_no, true, false, 8);
	pModel_->Update();
}

// �`��
void BossEnemy::Draw()
{
	// ���f���̕`��
	if (isDraw_)
	{
		pModel_->Draw();
	}

	// �f�o�b�O�p
#ifdef _DEBUG
	DrawSphere3D(pos_, collisionRadius_, 8, 0xff0000, 0xff0000, false);	
#endif
	Debug::Log("bossEnemy", pos_);
}

// UI�̕`��
void BossEnemy::DrawUI()
{
	// HP�o�[�̕`��
	pHpBar_->Draw(hp_bar_side_space, hp_bar_start_y, hp_bar_height);

	if (State::DEID == stateMachine_.GetCurrentState())
	{
		pTriangle_->Draw();
		pFlash_->Draw();
	}
}

// �_���[�W
void BossEnemy::OnDamage(int damage, VECTOR pos)
{
	// HP�����炷
	hp_ -= damage;

	// HP�o�[�̃o�[�����炷
	pHpBar_->OnDamage(hp_);

	// ���f���̃f�B�t���[�Y�J���[�𔽓]
	pModel_->InversAllMaterialDifColor();

	// �q�b�g�G�t�F�N�g�̍Đ�
	Effekseer3DEffectManager::GetInstance().PlayEffect(hitEffectHandle_, EffectID::enemy_boss_hit_effect, pos, {200.0f, 200.0f, 200.0f});

	// HP���Ȃ��Ȃ����ꍇ�A���S���̃X�e�[�g�ɕύX
	if (hp_ <= 0)
	{
		stateMachine_.SetState(State::DEID);
	}
}

// �ړ��̏�����
void BossEnemy::InitMove()
{
	// �n�_�̈ړ������̓���ւ�(�z��̒��g���V���b�t��)
	std::random_device seed;
	std::mt19937 engine(seed());
	std::shuffle(movePointTable_.begin(), movePointTable_.end(), engine);

	// ��ԍŏ��̖ڕW�n�_�̐ݒ�
	goalPos_ = movePointTable_.front();

	// ������
	movePoint_ = 0;
	isGoal_ = false;
	isMoveEnd_ = false;

	// �x�N�g���̍쐬
	VECTOR vec = VSub(goalPos_, pos_);
	vec = VNorm(vec);
	vec = VScale(vec, move_speed);
	moveVec_ = vec;
}

// �ړ�
void BossEnemy::Move()
{
	// �ړ����I�����Ă��Ȃ�������
	if (!isMoveEnd_)
	{
		// �ڕW�̒n�_�ɓ��B����
		if (isGoal_)
		{
			// ������
			isGoal_ = false;

			// �ڕW�n�_�̍X�V
			movePoint_++;

			// �S�Ă̒n�_���ړ�������
			if (static_cast<int>(movePointTable_.size()) <= movePoint_)
			{
				// �����ʒu�ɖ߂�����
				if (static_cast<int>(movePointTable_.size()) + 1 <= movePoint_)
				{
					// �ړ��I���t���O�𗧂Ă�
					isMoveEnd_ = true;

					// �O�̂��ߏ�����
					moveVec_ = { 0, 0, 0 };
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

// �L���[�u���[�U�[�̍U���̏���
void BossEnemy::CubeLaserAttack()
{
	// ���[�U�[�̔��ˈʒu�̃t���[�����W�̎擾
	VECTOR firePos = MV1GetFramePosition(pModel_->GetModelHandle(), cube_laser_fire_frame_1);

	// �v���C���[�Ɍ������x�N�g���̍쐬
	VECTOR vec = VSub(pPlayer_->GetPos(), firePos);
	vec = VNorm(vec);
	vec = VScale(vec, cubeLaserSpeed_);

	// ���[�U�[�̔���
	pLaserManager_->Create(LaserType::CUBE, &firePos, &vec);
}

// �X�e�[�g�̊Ǘ�
void BossEnemy::StateManager()
{
	stateMachine_.SetState(attackStateTable_[attackState_]);
	attackState_++;

	if (attackStateTable_.size() <= attackState_)
	{
		// �s���̏����̓���ւ�(�z��̒��g���V���b�t��)
		std::random_device seed;
		std::mt19937 engine(seed());
		std::shuffle(attackStateTable_.begin(), attackStateTable_.end(), engine);

		attackState_ = 0;
	}
}

////// Entar //////

void BossEnemy::EntarEntry()
{

}

void BossEnemy::EntarIdle()
{
	// �^�C�}�[�̐������Ԃ̐ݒ�
	utilTimerTable_["idleStateContinue"] = idle_state_continue_frame;

	// �A�j���[�V�����̕ύX
	pModel_->ChangeAnimation(idle_anim_no, true, false, 8);
}

void BossEnemy::EntarMove()
{
	InitMove();
}

void BossEnemy::EntarDamage()
{
}

void BossEnemy::EntarDied()
{
	moveVec_ = { 0, 0, 0 };

	pModel_->StopAnim();

	pTriangle_ = std::make_unique<Triangle>(5, died_continue_frame);
	pFlash_ = std::make_unique<Flash>(60);

	utilTimerTable_["effectIntarval"] = 20;
	utilTimerTable_["continueFrame"] = died_continue_frame;
}

void BossEnemy::EntarStopCubeLaserAttack()
{
	// �L���[�u���[�U�[�̔��ˊԊu�𐧌����ԂƂ��Đݒ�
	utilTimerTable_["cubeLaserInterval"] = cube_laser_interval_frame;

	// �L���[�u���[�U�[�̍U���̌p�����Ԃ𐧌����ԂƂ��Đݒ�
	utilTimerTable_["cubeLaserAttackContinue"] = cube_laser_attack_continue_frame;

	// �L���[�u���[�U�[���˗p�̃A�j���[�V�����ɕύX
	pModel_->ChangeAnimation(cube_laser_fire_anim_no, true, false, 8, 0.5f);
}

void BossEnemy::EntarStopNormalLaserAttack()
{
	// �ʏ탌�[�U�[���˗p�̃A�j���[�V�����ɕύX
	pModel_->ChangeAnimation(normal_laser_fire_anim_no, false, false, 8);

	// �ʏ탌�[�U�[�̔���
	pLaserManager_->Create(LaserType::NORMAL, &normalLaserFirePos_, &toTargetVec_, 180);
}

void BossEnemy::EntarMoveCubeLaserAttack()
{
	// �L���[�u���[�U�[���˗p�̃A�j���[�V�����ɕύX
	pModel_->ChangeAnimation(cube_laser_fire_anim_no, true, false, 8, 0.5f);

	// ������
	InitMove();
}

void BossEnemy::EntarMoveNormalLaserAttack()
{
	// �ʏ탌�[�U�[���˗p�̃A�j���[�V�����ɕύX
	pModel_->ChangeAnimation(normal_laser_fire_anim_no, true, false, 8);

	// �p�����[�U�[�̔���
	pLaserManager_->Create(LaserType::NORMAL_INFINITY, &normalLaserFirePos_, &toTargetVec_);

	// ������
	InitMove();
}

////// Update //////

void BossEnemy::UpdateEntry()
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

	// HP�o�[�̉��o���I�����A�v���C���[���ړI�̈ʒu�ɓ��B���Ă�����
	// �X�e�[�g��ύX
	if (pHpBar_->IsEndFirstDirection() &&
		pos_.z <= goal_init_pos.z)
	{
		stateMachine_.SetState(State::IDLE);
	}
}

void BossEnemy::UpdateIdle()
{
	// �ҋ@�X�e�[�g�̌p�����ԃ^�C�}�[�̍X�V
	utilTimerTable_["idleStateContinue"].Update(1);

	// �������Ԃ𒴂�����X�e�[�g�̕ύX
	if (utilTimerTable_["idleStateContinue"].IsTimeOut())
	{
	//	stateMachine_.SetState(State::MOVE_ATTACK_NORMAL_LASER);

		StateManager();
	}
}

void BossEnemy::UpdateMove()
{
	// �ړ�
	Move();

	// �ړ����I��������X�e�[�g�̕ύX
	if (isMoveEnd_)
	{
		stateMachine_.SetState(State::IDLE);
	}
}

void BossEnemy::UpdateDamage()
{
	stateMachine_.SetState(State::IDLE);
}

void BossEnemy::UpdateDied()
{
	// ���h��
	pos_.x += sinf(utilTimerTable_["continueFrame"].GetTime() * died_swing_speed) * died_swing_width;

	// ���X��Y����������
	pos_.y -= 0.5f;

	utilTimerTable_["continueFrame"].Update(1);
	if (!utilTimerTable_["continueFrame"].IsTimeOut())
	{
		// �G�t�F�N�g�̔����Ԋu�^�C�}�[�̍X�V
		utilTimerTable_["effectIntarval"].Update(1);
		if (utilTimerTable_["effectIntarval"].IsTimeOut())
		{
			// �G�t�F�N�g�̏��
			DieEffect dieEffect{};

			// �G�t�F�N�g�̔����ʒu���v���C���[�̎���Ƀ����_���ɐݒ�
			dieEffect.pos = 
			{
				// �G�t�F�N�g�̔����ʒu��-500�`500�̊ԂŃ����_���ɐݒ�
				pos_.x + GetRand(1000) - 500,
				pos_.y + GetRand(1000) - 500,
				pos_.z
			};

			// �G�t�F�N�g�̑傫����10�{����100�{�̊ԂŃ����_���ɐݒ�
			dieEffect.scale = GetRand(30) + 10;

			// xy�̃x�N�g���������_���쐬
			dieEffect.vec = { static_cast<float>(GetRand(10) - 5), static_cast<float>(GetRand(10) - 5), 0.0f };
			dieEffect.vec = VNorm(dieEffect.vec);
			dieEffect.vec = VScale(dieEffect.vec, 50.0f);

			// �G�t�F�N�g�̍Đ�
			Effekseer3DEffectManager::GetInstance().PlayEffectFollow(
				dieEffect.effectHandle,
				EffectID::enemy_died,
				&dieEffect.pos,
				{ dieEffect.scale, dieEffect.scale, dieEffect.scale },
				0.5f);

			dieEffectTable_.push_back(dieEffect);

			// �^�C�}�[�̏�����
			utilTimerTable_["effectIntarval"].Reset();
		}
		pTriangle_->Update(pos_);
	}
	else
	{
		isDraw_ = false;
		pTriangle_->SetDraw(false);
		pScreenEffect_->SetShake(100.0f, 0.0f, 180);
		pLaserManager_->GraduallyAlphaDeleteAllLaser();

		VECTOR pos = ConvWorldPosToScreenPos(pos_);
		pFlash_->Update({pos.x, pos.y}, 0xffffff);

		if (pFlash_->IsEnd())
		{
			// �C���X�^���X�̍폜
			isEnabled_ = false;
		}
	}

	// �G�t�F�N�g�̈ړ�
	for (auto& effect : dieEffectTable_)
	{
		effect.pos = VAdd(effect.pos, effect.vec);
	}
}

void BossEnemy::UpdateStopCubeLaserAttack()
{
	// �L���[�u���[�U�[�̍U���̌p�����ԃ^�C�}�[�̍X�V
	utilTimerTable_["cubeLaserAttackContinue"].Update(1);

	// �������ԓ��̊Ԃ̓L���[�u���[�U�[�𔭎�
	if (!utilTimerTable_["cubeLaserAttackContinue"].IsTimeOut())
	{
		// �L���[�u���[�U�[�̔��ˊԊu�^�C�}�[�̍X�V
		utilTimerTable_["cubeLaserInterval"].Update(1);

		// �������Ԃ𒴂����甭��
		if (utilTimerTable_["cubeLaserInterval"].IsTimeOut())
		{
			// �L���[�u���[�U�[�𔭎�
			CubeLaserAttack();

			// �^�C�}�[�̏�����
			utilTimerTable_["cubeLaserInterval"].Reset();
		}
	}
	else
	{
		// �p�����Ԃ̐������Ԃ𒴂�����X�e�[�g��ύX
		stateMachine_.SetState(State::IDLE);
	}
}

void BossEnemy::UpdateStopNormalLaserAttack()
{
	// �ʏ탌�[�U�[�p�̃A�j���[�V�������I��������X�e�[�g��ύX
	if (pModel_->IsAnimEnd())
	{
		// �X�e�[�g�̕ύX
		stateMachine_.SetState(State::IDLE);
	}
}

void BossEnemy::UpdateMoveCubeLaserAttack()
{
	// �ړ����I��������
	if (isMoveEnd_)
	{
		// �X�e�[�g��ύX
		stateMachine_.SetState(State::IDLE);
	}

	// �`�F�b�N�|�C���g�ɓ��B������
	if (isGoal_)
	{
		// �L���[�u���[�U�[����
		CubeLaserAttack();
	}

	// �ړ�
	Move();
}

void BossEnemy::UpdateMoveNormalLaserAttack()
{
	// �ړ�
	Move();

	// �ړ����I��������X�e�[�g�ύX
	if (isMoveEnd_)
	{
		stateMachine_.SetState(State::IDLE);
	}
}

////// Exit ////// 

void BossEnemy::ExitEntry()
{
}

void BossEnemy::ExitIdle()
{
	// �S�Ẵ^�C�}�[�̍폜
	utilTimerTable_.clear();
}

void BossEnemy::ExitMove()
{
	// ������
	moveVec_ = { 0, 0, 0 };
	isMoveEnd_ = false;
}

void BossEnemy::ExitDamage()
{

}

void BossEnemy::ExitDied()
{
}

void BossEnemy::ExitStopCubeLaserAttack()
{
	// �S�Ẵ^�C�}�[�̍폜
	utilTimerTable_.clear();
}

void BossEnemy::ExitStopNormalLaserAttack()
{
}

void BossEnemy::ExitMoveCubeLaserAttack()
{
	// ������
	isMoveEnd_ = false;
}

void BossEnemy::ExitMoveNormalLaserAttack()
{
	// ������
	isMoveEnd_ = false;

	// ���[�U�[�G�t�F�N�g�̍폜
	pLaserManager_->DeleteContinueLaser();
}