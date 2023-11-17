#include "BossEnemy.h"
#include "../Util/Model.h"
#include "../Util/Timer.h"
#include "../UI/HpBar.h"
#include "../Laser/LazerManager.h"
#include "../Player.h"
#include "../Util/Effekseer3DEffectManager.h"

namespace
{
	// HP
	constexpr int hp_bar_side_space = 300;
	constexpr int hp_bar_start_y = 50;
	constexpr int hp_bar_height = 30;
	constexpr float aim_hp_speed = 0.09f;
	constexpr float max_hp = 100.0f;

	// �ʏ탌�[�U�[�̔��ˈʒu�t���[��
	constexpr int normal_laser_fire_frame = 3;

	// �L���[�u���[�U�[�̔��ˈʒu�t���[��
	constexpr int cube_laser_fire_frame_1 = 4;
	constexpr int cube_laser_fire_frame_2 = 5;

	// �����ʒu
	constexpr VECTOR init_pos = { 0, 0, 5000 };

	// �g�嗦
	constexpr VECTOR scale = { 5, 5, 5 };

	// ��]��
	constexpr VECTOR rot = { 0, DX_PI_F, 0 };

	// �A�j���[�V�����ԍ�
	constexpr int idle_anim_no = 0;					// �ҋ@
	constexpr int normal_laser_fire_anim_no = 1;	// �ʏ탌�[�U�[�̔��ˎ�
	constexpr int cube_laser_fire_anim_no = 3;		// �L���[�u���[�U�[�̔��ˎ�

	// ���[�U�[�̔��ˊԊu
	constexpr int cube_laser_interval_frame = 60 * 2;
	constexpr int normal_laser_interval_frame = 60 * 10;
}

BossEnemy::BossEnemy(int modelHandle, std::shared_ptr<Player> pPlayer, std::shared_ptr<LazerManager> pLazerManager) 
{
	pModel_ = std::make_unique<Model>(modelHandle);
	pModel_->SetScale(scale);
	pModel_->SetAnimation(idle_anim_no, true, false);
	pHpBar_ = std::make_unique<HpBar>(max_hp);

	pPlayer_ = pPlayer;
	pLazerManager_ = pLazerManager;
	normalLaserFireIntervalTimer_ = normal_laser_interval_frame;
	cubeLaserFireIntervalTimer_ = cube_laser_interval_frame;
	pos_ = init_pos;
	rot_ = rot;
	lazerSpeed_ = 20.0f;
	hp_ = max_hp;

	pModel_->SetRot(rot_);
	pModel_->SetPos(pos_);
	pModel_->Update();
}

BossEnemy::~BossEnemy()
{
}

void BossEnemy::Update()
{
	// HP�o�[�̉��o���I������ꍇ
	if (pHpBar_->IsEndFirstDirection())
	{
		cubeLaserFireIntervalTimer_.Update(1);
		if (cubeLaserFireIntervalTimer_.IsTimeOut())
		{
			// ���[�U�[�̔��ˈʒu�̃t���[�����W�̎擾
			VECTOR firePos = MV1GetFramePosition(pModel_->GetModelHandle(), cube_laser_fire_frame_1);

			VECTOR vec = VSub(pPlayer_->GetPos(), firePos);
			vec = VNorm(vec);
			vec = VScale(vec, lazerSpeed_);
			pLazerManager_->Create(LazerType::CUBE, &firePos, &vec, {});
			cubeLaserFireIntervalTimer_.Reset();
		}
	}

	pHpBar_->Update(aim_hp_speed);

	pModel_->SetRot(rot_);
	pModel_->SetPos(pos_);
	pModel_->Update();
}

void BossEnemy::Draw()
{
	pModel_->Draw();

	pHpBar_->Draw(hp_bar_side_space, hp_bar_start_y, hp_bar_height);
}

void BossEnemy::OnDamage(int damage, VECTOR pos)
{
	hp_ -= damage;

	pHpBar_->OnDamage(hp_);

	Effekseer3DEffectManager::GetInstance().PlayEffect(
		onDamageEffectHandle_,
		EffectID::enemy_on_damage,
		pos,
		200.0f,
		0.5f);
}
