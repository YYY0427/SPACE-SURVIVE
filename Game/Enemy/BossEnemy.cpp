#include "BossEnemy.h"
#include "../Util/Model.h"
#include "../Util/Timer.h"
#include "../UI/HpBar.h"
#include "../Laser/LazerManager.h"
#include "../Player.h"
#include "../Util/Effekseer3DEffectManager.h"
#include "../Util/Debug.h";
#include "../Util/MathUtil.h"
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

	// モデルの向いている方向
	constexpr VECTOR init_model_direction = { 0, 0, -1 };

	// アニメーション番号
	constexpr int idle_anim_no = 0;					// 待機
	constexpr int normal_laser_fire_anim_no = 1;	// 通常レーザーの発射時
	constexpr int cube_laser_fire_anim_no = 3;		// キューブレーザーの発射時

	// レーザー攻撃を何フレーム行うか
	constexpr int cube_laser_attack_continue_frame = 60 * 10;

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

	// 待機ステートの継続時間フレーム
	constexpr int idle_state_continue_frame = 180;
}

// コンストラクタ
BossEnemy::BossEnemy(int modelHandle, std::shared_ptr<Player> pPlayer, std::shared_ptr<LazerManager> pLazerManager) :
	isGoal_(false),
	isMoveEnd_(false),
	movePoint_(0)
{
	// 初期化
	pPlayer_ = pPlayer;
	pLaserManager_ = pLazerManager;
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

	// ステートの初期化
	InitState();

	// ステートの設定
	stateMachine_.SetState(State::ENTRY);

	// インスタンス生成
	pModel_ = std::make_unique<Model>(modelHandle);
	pHpBar_ = std::make_unique<HpBar>(max_hp);

	// モデルの設定
	pModel_->SetOpacity(opacity_);	// 不透明度
	pModel_->SetScale(model_scale);	// 拡大率
	pModel_->SetRot(rot_);			// 回転率
	pModel_->SetPos(pos_);			// 位置
	pModel_->Update();				// 当たり判定、アニメーションの更新
}

// デストラクタ
BossEnemy::~BossEnemy()
{
}

// ステートの更新
void BossEnemy::InitState()
{
	// ステートマシン初期化
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
}

// 更新
void BossEnemy::Update()
{
	// ステートマシンの更新
	stateMachine_.Update();

	// 通常レーザーの発射位置のフレーム座標の取得
	normalLaserFirePos_ = MV1GetFramePosition(pModel_->GetModelHandle(), normal_laser_fire_frame);

	// TODO :　プレイヤーに向かうベクトルを下を使って実装する
	pPlayer_->GetPosLogTable();

	// 通常レーザがプレイヤーに向かうベクトルの作成
	toTargetVec_ = VSub(pPlayer_->GetPosLogTable().back(), normalLaserFirePos_);
	toTargetVec_ = VNorm(toTargetVec_);

	// 登場中の場合はプレイヤーの方向を向かない
	if(stateMachine_.GetCurrentState() != State::ENTRY)
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
	pModel_->SetOpacity(opacity_);	// 不透明度
	pModel_->SetRot(rot_);			// 回転率
	pModel_->SetPos(pos_);			// 位置
	pModel_->Update();				// モデルの当たり判定、アニメーションの更新
}

// 描画
void BossEnemy::Draw()
{
	// モデルの描画
	pModel_->Draw();

	// デバッグ用
#ifdef _DEBUG
	DrawSphere3D(pos_, collisionRadius_, 8, 0xff0000, 0xff0000, false);	
#endif
	Debug::Log("bossEnemy", pos_);
}

// UIの描画
void BossEnemy::DrawUI()
{
	// HPバーの描画
	pHpBar_->Draw(hp_bar_side_space, hp_bar_start_y, hp_bar_height);
}

// ダメージ
void BossEnemy::OnDamage(int damage, VECTOR pos)
{
	// HPを減らす
	hp_ -= damage;

	// HPバーのバーを減らす
	pHpBar_->OnDamage(hp_);

	// HPがなくなった場合、死亡時のステートに変更
	if (hp_ <= 0)
	{
		stateMachine_.SetState(State::DEID);
	}
	// HPが残っていたら、ダメージを受けた時のステートに変更
	else
	{
		stateMachine_.SetState(State::DAMAGE);
	}
}

// 移動の初期化
void BossEnemy::InitMove()
{
	// 地点の移動順序の入れ替え(配列の中身をシャッフル)
	std::random_device seed;
	std::mt19937 engine(seed());
	std::shuffle(movePointTable_.begin(), movePointTable_.end(), engine);

	// 一番最初の目標地点の設定
	goalPos_ = movePointTable_.front();

	// 初期化
	movePoint_ = 0;
	isGoal_ = false;
	isMoveEnd_ = false;

	// ベクトルの作成
	VECTOR vec = VSub(goalPos_, pos_);
	vec = VNorm(vec);
	vec = VScale(vec, move_speed);
	moveVec_ = vec;
}

// 移動
void BossEnemy::Move()
{
	// 移動が終了していなかったら
	if (!isMoveEnd_)
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
					// 移動終了フラグを立てる
					isMoveEnd_ = true;

					// 念のため初期化
					moveVec_ = { 0, 0, 0 };
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

// キューブレーザーの攻撃の処理
void BossEnemy::CubeLaserAttack()
{
	// レーザーの発射位置のフレーム座標の取得
	VECTOR firePos = MV1GetFramePosition(pModel_->GetModelHandle(), cube_laser_fire_frame_1);

	// プレイヤーに向かうベクトルの作成
	VECTOR vec = VSub(pPlayer_->GetPos(), firePos);
	vec = VNorm(vec);
	vec = VScale(vec, cubeLaserSpeed_);

	// レーザーの発射
	pLaserManager_->Create(LaserType::CUBE, &firePos, &vec);
}

////// Entar //////

void BossEnemy::EntarEntry()
{

}

void BossEnemy::EntarIdle()
{
	// タイマーの制限時間の設定
	utilTimerTable_["idleStateContinue"] = idle_state_continue_frame;

	// アニメーションの変更
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
	Effekseer3DEffectManager::GetInstance().PlayEffect(
		onDamageEffectHandle_,
		EffectID::enemy_died,
		pos_,
		{ 200.0f, 200.0f, 200.0f },
		0.5f);
}

void BossEnemy::EntarStopCubeLaserAttack()
{
	// キューブレーザーの発射間隔を制限時間として設定
	utilTimerTable_["cubeLaserInterval"] = cube_laser_interval_frame;

	// キューブレーザーの攻撃の継続時間を制限時間として設定
	utilTimerTable_["cubeLaserAttackContinue"] = cube_laser_attack_continue_frame;

	// キューブレーザー発射用のアニメーションに変更
	pModel_->ChangeAnimation(cube_laser_fire_anim_no, true, false, 8, 0.5f);
}

void BossEnemy::EntarStopNormalLaserAttack()
{
	// 通常レーザー発射用のアニメーションに変更
	pModel_->ChangeAnimation(normal_laser_fire_anim_no, false, false, 8);

	// 通常レーザーの発射
	pLaserManager_->Create(LaserType::NORMAL, &normalLaserFirePos_, &toTargetVec_);
}

void BossEnemy::EntarMoveCubeLaserAttack()
{
	// キューブレーザー発射用のアニメーションに変更
	pModel_->ChangeAnimation(cube_laser_fire_anim_no, true, false, 8, 0.5f);

	// 初期化
	InitMove();
}

void BossEnemy::EntarMoveNormalLaserAttack()
{
	// 通常レーザー発射用のアニメーションに変更
	pModel_->ChangeAnimation(normal_laser_fire_anim_no, true, false, 8);

	// 継続レーザーの発射
	pLaserManager_->Create(LaserType::CONTINUE_NORMAL, &normalLaserFirePos_, &toTargetVec_);

	// 初期化
	InitMove();
}

////// Update //////

void BossEnemy::UpdateEntry()
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

	// HPバーの演出が終了し、プレイヤーが目的の位置に到達していたら
	// ステートを変更
	if (pHpBar_->IsEndFirstDirection() &&
		pos_.z <= goal_init_pos.z)
	{
		stateMachine_.SetState(State::IDLE);
	}
}

void BossEnemy::UpdateIdle()
{
	// 待機ステートの継続時間タイマーの更新
	utilTimerTable_["idleStateContinue"].Update(1);

	// 制限時間を超えたらステートの変更
	if (utilTimerTable_["idleStateContinue"].IsTimeOut())
	{
		stateMachine_.SetState(State::MOVE_ATTACK_NORMAL_LASER);
	}
}

void BossEnemy::UpdateMove()
{
	// 移動
	Move();

	// 移動が終了したらステートの変更
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
}

void BossEnemy::UpdateStopCubeLaserAttack()
{
	// キューブレーザーの攻撃の継続時間タイマーの更新
	utilTimerTable_["cubeLaserAttackContinue"].Update(1);

	// 制限時間内の間はキューブレーザーを発射
	if (!utilTimerTable_["cubeLaserAttackContinue"].IsTimeOut())
	{
		// キューブレーザーの発射間隔タイマーの更新
		utilTimerTable_["cubeLaserInterval"].Update(1);

		// 制限時間を超えたら発射
		if (utilTimerTable_["cubeLaserInterval"].IsTimeOut())
		{
			// キューブレーザーを発射
			CubeLaserAttack();

			// タイマーの初期化
			utilTimerTable_["cubeLaserInterval"].Reset();
		}
	}
	else
	{
		// 継続時間の制限時間を超えたらステートを変更
		stateMachine_.SetState(State::IDLE);
	}
}

void BossEnemy::UpdateStopNormalLaserAttack()
{
	// 通常レーザー用のアニメーションが終了したらステートを変更
	if (pModel_->IsAnimEnd())
	{
		// ステートの変更
		stateMachine_.SetState(State::IDLE);
	}
}

void BossEnemy::UpdateMoveCubeLaserAttack()
{
	// 移動が終了したら
	if (isMoveEnd_)
	{
		// ステートを変更
		stateMachine_.SetState(State::IDLE);
	}

	// チェックポイントに到達したら
	if (isGoal_)
	{
		// キューブレーザー発射
		CubeLaserAttack();
	}

	// 移動
	Move();
}

void BossEnemy::UpdateMoveNormalLaserAttack()
{
	// 移動
	Move();

	// 移動が終了したらステート変更
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
	// 全てのタイマーの削除
	utilTimerTable_.clear();
}

void BossEnemy::ExitMove()
{
	// 初期化
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
	// 全てのタイマーの削除
	utilTimerTable_.clear();
}

void BossEnemy::ExitStopNormalLaserAttack()
{
}

void BossEnemy::ExitMoveCubeLaserAttack()
{
	// 初期化
	isMoveEnd_ = false;
}

void BossEnemy::ExitMoveNormalLaserAttack()
{
	// 初期化
	isMoveEnd_ = false;

	// TODO : レーザーエフェクトの削除
	pLaserManager_->DeleteContinueLaser();
}