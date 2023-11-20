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

	// キューブレーザーの発射位置フレーム
	constexpr int cube_laser_fire_frame_1 = 4;
	constexpr int cube_laser_fire_frame_2 = 5;

	// 通常レーザーの発射位置フレーム
	constexpr int normal_laser_fire_frame = 2;

	// 初期位置
	constexpr VECTOR start_init_pos = { 0, 300, 1100 };
	constexpr VECTOR goal_init_pos = { 0, 300, 800 };

	// 拡大率
	constexpr VECTOR scale = { 1, 1, 1 };

	// 回転率
	constexpr VECTOR rot = { 20 * DX_PI_F / 180.0f, DX_PI_F, 0 };

	// アニメーション番号
	constexpr int idle_anim_no = 0;					// 待機
	constexpr int normal_laser_fire_anim_no = 1;	// 通常レーザーの発射時
	constexpr int cube_laser_fire_anim_no = 3;		// キューブレーザーの発射時

	// レーザーの発射間隔
	constexpr int cube_laser_interval_frame = 30;
	constexpr int normal_laser_interval_frame = 60 * 10;

	// レーザーの速度
	constexpr float cube_laser_speed = 20.0f;	
	constexpr float normal_laser_speed = 20.0f;
}

BossEnemy::BossEnemy(int modelHandle, std::shared_ptr<Player> pPlayer, std::shared_ptr<LazerManager> pLazerManager) :
	updateFunc_(&BossEnemy::EntryUpdate),
	entryEffectH_(-1)
{
	pPlayer_ = pPlayer;
	pLazerManager_ = pLazerManager;
	normalLaserFireIntervalTimer_ = normal_laser_interval_frame;
	cubeLaserFireIntervalTimer_ = cube_laser_interval_frame;
	pos_ = start_init_pos;
	rot_ = rot;
	cubeLaserSpeed_ = cube_laser_speed;
	normalLaserSpeed_ = normal_laser_speed;
	hp_ = max_hp;
	opacity = 0.0f;
	moveVec_ = { 0, 0, 0 };

	pModel_ = std::make_unique<Model>(modelHandle);
	pHpBar_ = std::make_unique<HpBar>(max_hp);

	MV1SetOpacityRate(pModel_->GetModelHandle(), opacity);
	pModel_->SetScale(scale);
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

	VECTOR toPlayerVec = VSub(pos_, pPlayer_->GetPos());
//	rot_.y = atan2(toPlayerVec.x, toPlayerVec.z) + DX_PI_F;

	pos_ = VAdd(pos_, moveVec_);
	pHpBar_->Update(aim_hp_speed);
	MV1SetOpacityRate(pModel_->GetModelHandle(), opacity);
	pModel_->SetRot(rot_);
	pModel_->SetPos(pos_);
	pModel_->Update();
}

void BossEnemy::EntryUpdate()
{
	opacity += 0.005f;

	if (pos_.z >= goal_init_pos.z)
	{
		pos_.z -= 1.5f;
	}
	else
	{
		pModel_->SetAnimation(idle_anim_no, true, false);
	}

	// HPバーの演出が終わった場合
	if (pHpBar_->IsEndFirstDirection() && pos_.z <= goal_init_pos.z)
	{
		updateFunc_ = &BossEnemy::NormalLaserAttackUpdate;
	}
}

void BossEnemy::CubeLaserAttackUpdate()
{
	// 一定間隔でキューブレーザーの発射
	cubeLaserFireIntervalTimer_.Update(1);
	if (cubeLaserFireIntervalTimer_.IsTimeOut())
	{
		pModel_->ChangeAnimation(cube_laser_fire_anim_no, false, false, 8);

		if (!pModel_->IsAnimEnd())
		{
			// レーザーの発射位置のフレーム座標の取得
			VECTOR firePos = MV1GetFramePosition(pModel_->GetModelHandle(), cube_laser_fire_frame_1);

			// プレイヤーに向かうベクトルの作成
			VECTOR vec = VSub(pPlayer_->GetPos(), firePos);
			vec = VNorm(vec);
			vec = VScale(vec, normalLaserSpeed_);

			// レーザーの発射
			pLazerManager_->Create(LazerType::CUBE, &firePos, &vec, {});

			// タイマーの初期化
			cubeLaserFireIntervalTimer_.Reset();
		}
	}
}

void BossEnemy::NormalLaserAttackUpdate()
{
	pModel_->ChangeAnimation(normal_laser_fire_anim_no, false, false, 8);

	// レーザーの発射位置のフレーム座標の取得
	firePos_ = MV1GetFramePosition(pModel_->GetModelHandle(), normal_laser_fire_frame);

	// プレイヤーに向かうベクトルの作成
	toPlayerVec_ = VSub(pPlayer_->GetPos(), firePos_);
	toPlayerVec_ = VNorm(toPlayerVec_);
	toPlayerVec_ = VScale(toPlayerVec_, normalLaserSpeed_);

	// キューブレーザーの発射
//	normalLaserFireIntervalTimer_.Update(1);
//	if (normalLaserFireIntervalTimer_.IsTimeOut())
	{
		if (pModel_->IsAnimEnd())
		{
			// レーザーの発射
			pLazerManager_->Create(LazerType::NORMAL, &firePos_, &toPlayerVec_, &moveVec_);

			// タイマーの初期化
			normalLaserFireIntervalTimer_.Reset();

			updateFunc_ = &BossEnemy::CubeLaserAttackUpdate;
		}
	}
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
		EffectID::enemy_died,
		pos,
		200.0f,
		0.5f);
}
