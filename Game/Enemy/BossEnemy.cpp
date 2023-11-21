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

	// キューブレーザーの発射位置フレーム
	constexpr int cube_laser_fire_frame_1 = 4;
	constexpr int cube_laser_fire_frame_2 = 5;

	// 通常レーザーの発射位置フレーム
	constexpr int normal_laser_fire_frame = 2;

	// 初期位置
	constexpr VECTOR start_init_pos = { 0, 300, 1100 };
	constexpr VECTOR goal_init_pos = { 0, 300, 800 };

	// 拡大率
	constexpr VECTOR model_scale = { 1, 1, 1 };

	// 回転率
	constexpr VECTOR rot = { 20 * DX_PI_F / 180.0f, DX_PI_F, 0 };

	constexpr VECTOR init_model_direction = { 0, 0, -1 };

	// アニメーション番号
	constexpr int idle_anim_no = 0;					// 待機
	constexpr int normal_laser_fire_anim_no = 1;	// 通常レーザーの発射時
	constexpr int cube_laser_fire_anim_no = 3;		// キューブレーザーの発射時

	// レーザーの発射間隔
	constexpr int cube_laser_interval_frame = 60 * 2;
	constexpr int normal_laser_interval_frame = 60 * 10;

	// レーザーの速度
	constexpr float cube_laser_speed = 5.0f;	

	// 当たり判定の半径
	constexpr float collision_radius = 250.0f;

	// 移動する地点
	constexpr VECTOR move_pos[] =
	{
		goal_init_pos,
		{ 930, 300, 800 },
		{ 930, -300, 800 },
		{ -930, 300, 800 },
		{ -930, -300, 800 },
	};

	// 移動する速度
	constexpr float move_speed = 10.0f;

	// 移動の誤差範囲
	// 目標の地点に到達したか確かめる時、誤差が発生するため誤差の範囲を設定する
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

	// 通常レーザーの発射位置のフレーム座標の取得
	normalLaserFirePos_ = MV1GetFramePosition(pModel_->GetModelHandle(), normal_laser_fire_frame);

	// 通常レーザがプレイヤーに向かうベクトルの作成
	toTargetVec_ = VSub(pPlayer_->GetPos(), normalLaserFirePos_);
	toTargetVec_ = VNorm(toTargetVec_);

	// 登場中の場合はプレイヤーの方向を向かない
	if (updateFunc_ != &BossEnemy::EntryUpdate)
	{
		// ベクトル方向の回転行列を角度に変換
		bool isGimbalLock = false;
		MATRIX rotMtx = MGetRotVec2(init_model_direction, toTargetVec_);
		rot_ = MathUtil::ToEulerAngles(rotMtx, isGimbalLock);
		rot_.y += DX_PI_F;
		rot_.x *= -1;
		rot_.z *= -1;
	}

	// 位置の更新
	pos_ = VAdd(pos_, moveVec_);

	// HPバーの更新
	pHpBar_->Update(aim_hp_speed);

	// モデル設定
	pModel_->SetOpacity(opacity_);	// 不透明度の設定
	pModel_->SetRot(rot_);			// 回転率の設定
	pModel_->SetPos(pos_);			// 位置情報の設定
	pModel_->Update();				// モデルの当たり判定、アニメーションの更新
}

void BossEnemy::EntryUpdate()
{
	// 不透明度を徐々に大きくする
	opacity_ += 0.005f;

	// 目標の地点まで徐々に近づく
	if (pos_.z >= goal_init_pos.z)
	{
		pos_.z -= 1.5f;
	}
	else
	{
		// 目標の地点までの到達したらアニメーション開始
		pModel_->SetAnimation(idle_anim_no, true, false);
	}

	// HPバーの演出が終わった場合
	if (pHpBar_->IsEndFirstDirection() && pos_.z <= goal_init_pos.z)
	{
		updateFunc_ = &BossEnemy::CubeLaserAttackUpdate;
	}
}

void BossEnemy::IdleUpdate()
{
	pModel_->ChangeAnimation(idle_anim_no, true, false, 8);
}

void BossEnemy::CubeLaserAttackUpdate()
{
	// 一定間隔でキューブレーザーの発射
	cubeLaserFireIntervalTimer_.Update(1);
	if (cubeLaserFireIntervalTimer_.IsTimeOut())
	{
		pModel_->ChangeAnimation(cube_laser_fire_anim_no, false, false, 8, 0.5f);

		if (!pModel_->IsAnimEnd())
		{
			// レーザーの発射位置のフレーム座標の取得
			VECTOR firePos = MV1GetFramePosition(pModel_->GetModelHandle(), cube_laser_fire_frame_1);

			// プレイヤーに向かうベクトルの作成
			VECTOR vec = VSub(pPlayer_->GetPos(), firePos);
			vec = VNorm(vec);
			vec = VScale(vec, cubeLaserSpeed_);

			// レーザーの発射
			pLazerManager_->Create(LazerType::CUBE, &firePos, &vec, {});

			// タイマーの初期化
			cubeLaserFireIntervalTimer_.Reset();
		}
	}
}

void BossEnemy::NormalLaserAttackUpdate()
{
	// 通常レーザー発射用のアニメーションに変更
	pModel_->ChangeAnimation(normal_laser_fire_anim_no, false, false, 8);

	// 通常レーザーの発射
	if (!isNormalLaser_)
	{
		pLazerManager_->Create(LazerType::NORMAL, &normalLaserFirePos_, &toTargetVec_, &moveVec_);
		isNormalLaser_ = true;
	}

	// アニメーションが終了した場合
	if (pModel_->IsAnimEnd())
	{
		// 初期化
		isNormalLaser_ = false;

		// updateを変更
		updateFunc_ = &BossEnemy::IdleUpdate;
	}
}

void BossEnemy::MoveUpdate()
{
	if (!isMoveUpdateInit_)
	{
		// 地点の移動順序の入れ替え(配列の中身をシャッフル)
		std::random_device seed;
		std::mt19937 engine(seed());
		std::shuffle(movePointTable_.begin(), movePointTable_.end(), engine);

		// 一番最初の目標地点の設定
		goalPos_ = movePointTable_.front();

		// 初期化
		movePoint_ = 0;

		// ベクトルの作成
		VECTOR vec = VSub(goalPos_, pos_);
		vec = VNorm(vec);
		vec = VScale(vec, move_speed);
		moveVec_ = vec;
		
		// この処理に入らないようフラグ立て
		isMoveUpdateInit_ = true;
	}
	else
	{
		// 目標の地点に到達した
		if (isGoal_)
		{
			// 初期化
			isGoal_ = false;

			// 目標地点の更新
			movePoint_++;

			// 全ての地点を移動したか
			if (static_cast<int>(movePointTable_.size()) <= movePoint_)
			{
				// 初期位置に戻ったか
				if (static_cast<int>(movePointTable_.size()) + 1 <= movePoint_)
				{
					// 初期化
					moveVec_ = { 0, 0, 0 };
					isMoveUpdateInit_ = false;

					// updateの変更
					updateFunc_ = &BossEnemy::IdleUpdate;
					return;
				}

				// 初期位置を目標地点に設定
				goalPos_ = goal_init_pos;
			}
			else
			{
				// 順番に地点の取得
				goalPos_ = movePointTable_.at(movePoint_);
			}
			// ベクトルの作成
			VECTOR vec = VSub(goalPos_, pos_);
			vec = VNorm(vec);
			vec = VScale(vec, move_speed);
			moveVec_ = vec;
		}
		// 目標の地点に到達してない
		else
		{
			// 目標の地点に到達したらフラグを立てる
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
