#include "Player.h"
#include "Camera.h"
#include "Util/InputState.h"
#include "Util/Model.h"
#include "Util//Effekseer3DEffectManager.h"
#include <string>

namespace
{
	// プレイヤーモデルのファイルのパス
	std::string model_file_path = "Data/Model/MV1/Player.mv1";

	// プレイヤーの移動量
	constexpr VECTOR player_vec_up = { 0, 0, -1 };
	constexpr VECTOR player_vec_down = { 0, 0, 1 };
	constexpr VECTOR player_vec_right = { 1, 0, 0 };
	constexpr VECTOR player_vec_left = { -1, 0, 0 };

	// モデルの拡大率
	constexpr float model_scale = 0.5f;

	// プレイヤーの通常移動速度
	constexpr float move_normal_speed = 1.5f;

	// プレイヤーのブースト時移動速度
	constexpr float move_boost_speed = 3.0f;

	// プレイヤーの上昇、下降スピード
	constexpr float move_y_speed = 5.0f;

	// スローモーションのレート
	constexpr float slow_rate = 0.1f;

	// スローモーション時のエネルギー消費量
	constexpr float slow_energy_cost = 3.0f;

	// ブースト時のエネルギー消費量
	constexpr float boost_energy_cost = 1.0f;

	// エネルギーゲージ
	constexpr float energy_gauge_total_amount = 1000.0f;

	// エネルギーの回復量
	constexpr float energy_recovery_amount = 10.0f;

	// プレイヤーの当たり判定の半径
	constexpr float model_collision_radius = 30.0f;

	// 落下死亡判定の高さ
	constexpr float death_judg_height = -1000.0f;

	// 無敵時間のフレーム数
	constexpr int ultimate_frames = 120;

	// HP
	constexpr int hp = 5;
}

//  コンストラクタ
Player::Player(UnityGameObject data) :
	pos_(data.pos),
	rot_(data.rot),
	moveVec_(VGet(0.0f, 0.0f, 0.0f)),
	hp_(hp),
	ultimateTimer_(0),
	isInput_(false),
	moveSpeed_(move_normal_speed),
	energyGauge_(energy_gauge_total_amount),
	slowRate_(1.0f),
	boostEffectScale_(50.0f), 
	isPlayGameOverEffect_(false),
	isBoost_(false),
	isSlow_(false),
	isReverseMoveVec_(false)
{
	// プレイヤーモデルのインスタンスの作成
	pModel_ = std::make_shared<Model>(model_file_path.c_str());

	// モデルの拡大率の設定
//	pModel_->SetScale(data.scale);
	pModel_->SetScale(VGet(model_scale, model_scale, model_scale));

	// 回転率の設定
	pModel_->SetRot(rot_);

	// 位置情報の設定
	pModel_->SetPos(pos_);

	// アニメーションと当たり判定の更新
	pModel_->Update();
}

//  デストラクタ
Player::~Player()
{
	// 処理なし
}

// 更新
void Player::Update()
{
	// 左スティックの入力情報の取得
	int up = InputState::IsPadStick(PadLR::LEFT, PadStickInputType::UP);
	int down = InputState::IsPadStick(PadLR::LEFT, PadStickInputType::DOWN);
	int left = InputState::IsPadStick(PadLR::LEFT, PadStickInputType::LEFT);
	int right = InputState::IsPadStick(PadLR::LEFT, PadStickInputType::RIGHT);

	// カメラが向いている方向からベクトル変換して移動情報作成
#if true
	VECTOR moveForward = VScale(VNorm(VSub(pCamera_->GetTarget(), pCamera_->GetPos())), -1);
	VECTOR moveBack = VScale(moveForward, -1);
#else 
	VECTOR moveForward = VTransform(player_vec_up, MGetRotY(pCamera_->GetCameraYaw()));
	VECTOR moveBack = VTransform(player_vec_down, MGetRotY(pCamera_->GetCameraYaw()));
#endif
	VECTOR moveRight = VTransform(player_vec_right, MGetRotY(pCamera_->GetCameraYaw()));
	VECTOR moveLeft = VTransform(player_vec_left, MGetRotY(pCamera_->GetCameraYaw()));

	// スローモーションの処理
	SlowProcess();

	// ブーストの処理
	BoostProcess();
	
	// エネルギーの処理
	EnergyProcess();

	// 移動情報の初期化
	isInput_ = false;
	moveVec_ = VGet(0, 0, 0);
	VECTOR moveVecX = VGet(0, 0, 0);
	VECTOR moveVecZ = VGet(0, 0, 0);

	// スティックが入力されていたら移動ベクトルにスティックが傾いている方向のベクトルを代入
	// スティックの傾きぐわいによってベクトルが大きくなる
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

	// スティックが入力されている場合のみ移動
	if (isInput_)
	{
		// プレイヤーから見てx方向とz方向のベクトルを足して移動ベクトルを作成する
		moveVec_ = VAdd(moveVecZ, moveVecX);

		// プレイヤーのスピードを掛ける
		moveVec_ = VScale(moveVec_, moveSpeed_);

		// スローモーションのレートを掛ける
		moveVec_ = VScale(moveVec_, slowRate_);

		// 作成した移動ベクトルで座標の移動
		pos_ = VAdd(pos_, moveVec_);
	}
	else
	{
		// 動いてない場合かつスローモーションじゃない場合は通常スピードに切り替える
		if (!isSlow_)
		{
			isBoost_ = false;
		}
	}

	// 無敵時間のタイマーの更新
	// 0以下にはならない
	ultimateTimer_ = (std::max)(--ultimateTimer_, 0);

	// 移動ベクトルの大きさからプレイヤーの傾き具合を算出
	rot_ = VGet(moveVec_.z * DX_PI_F / 180.0f / slowRate_, 0.0f, -moveVec_.x * DX_PI_F / 180.0f / slowRate_);

	// 位置座標の設定
	pModel_->SetPos(pos_);

	// 向いている方向の設定
	pModel_->SetRot(rot_);

	// アニメーションを進める
	pModel_->Update();
}

// 岩との衝突時の更新
bool Player::CollisionRockUpdate()
{
	// ブースト時のエフェクトの再生のストップ
//	Effekseer3DEffectManager::GetInstance().StopEffect("starFire");
	
	// 移動ベクトルを反転していなかったら反転
	// 既に反転していたら反転しない
	if (!isReverseMoveVec_)
	{
		isReverseMoveVec_ = true;
		moveVec_ = VScale(moveVec_, -1.0f);
	}
	// ベクトルを徐々に小さくする
	moveVec_ = VScale(moveVec_, 0.96f);

	// 作成した移動ベクトルで座標の移動
	pos_ = VAdd(pos_, moveVec_);

	// ベクトルが特定の大きさよりも小さくなったらゲームオーバーエフェクト再生
	if(VSize(moveVec_) <= 1.0f)
	{
		// ダメージ処理
		OnDamage();

		if (hp_ <= 0)
		{
			// ゲームオーバーエフェクトを再生してなかったら再生
			// 既に再生していたら再生しない
			if (!isPlayGameOverEffect_)
			{
				isPlayGameOverEffect_ = true;
				float scale = 50.0f, speed = 0.5f;
				VECTOR rot{ 0.0f, 0.0f, 0.0f };
				Effekseer3DEffectManager::GetInstance().PlayEffect("explosion2", PlayType::NORMAL, &pos_, &scale, &speed, &rot);
			}
			// エフェクトを再生し終えたらtrueを返す
//			if (!Effekseer3DEffectManager::GetInstance().IsPlayingEffect("explosion2") && isPlayGameOverEffect_)
			{
				// 処理終了
				return true;
			}
		}
		else
		{
			// 初期化
			isReverseMoveVec_ = false;

			// 処理終了
			return true;
		}
	}
	
	// 位置座標の設定
	pModel_->SetPos(pos_);

	// 向いている方向の設定
	pModel_->SetRot(VGet(VSize(moveVec_) / 5.0f, 0.0f, VSize(moveVec_) / 10.0f));

	// アニメーションと当たり判定の更新
	pModel_->Update();

	// 処理が途中なのでfalseを返す
	return false;
}

// ブーストの処理
void Player::BoostProcess()
{
	// ブースト切り替え
	if (InputState::IsTriggered(InputType::BOOST))
	{
		// 非ブースト時かつエネルギーゲージの残量があった場合ブーストに移行
		if (!isBoost_ && energyGauge_ > 0)
		{
			isBoost_ = true;

			// ブースト時のエフェクトを再生
			VECTOR rot{ 0.0f, 0.0f, 0.0f };
			Effekseer3DEffectManager::GetInstance().PlayEffect("starFire", PlayType::LOOP_AND_FOLLOW, &pos_, &boostEffectScale_, &slowRate_, &rot);
		}
		// ブースト時の場合は通常速度に移行
		else
		{
			isBoost_ = false;
		}
	}
	// ブースト時
	if (isBoost_)
	{
		// スローモーション時はブーストのエフェクトの大きさを小さくする
		// 通常時は通常のエフェクトの大きさ
		(isSlow_) ?
			(boostEffectScale_ = 25.0f) :
			(boostEffectScale_ = 50.0f);

		// 徐々に加速
		moveSpeed_ += 1.0f;

		// 特定のスピードより大きくならない
		if (moveSpeed_ > move_boost_speed)
		{
			moveSpeed_ = move_boost_speed;
		}
	}
	// 非ブースト時
	else
	{
		// ブースト時のエフェクトの再生をストップ
	//	Effekseer3DEffectManager::GetInstance().StopEffect("starFire");

		// 徐々に減速
		moveSpeed_ -= 1.0f;

		// 特定のスピードより小さくならない
		if (moveSpeed_ < move_normal_speed)
		{
			moveSpeed_ = move_normal_speed;
		}
	}
}

// スローモーションの処理
void Player::SlowProcess()
{
	// スローモーション切り替え
	if (InputState::IsTriggered(InputType::SLOW))
	{
		// スローモーションじゃないかつエネルギーゲージの残量があったらスローモーションに移行
		if (!isSlow_ && energyGauge_ > 0)
		{
			isSlow_ = true;
		}
		// スローモーション時は通常に移行
		else
		{
			isSlow_ = false;
		}
	}
	// フラグが立っていたらスローモーションに切り替え
	if (isSlow_)
	{
		// 徐々にスローにしていく
		slowRate_ -= 0.1f;

		// 特定の値よりは小さくならない
		if (slowRate_ < slow_rate)
		{
			slowRate_ = slow_rate;
		}
	}
	else
	{
		// 徐々に速くしていく
		slowRate_ += 0.1f;

		// 特定の値よりは大きくならない
		if (slowRate_ >= 1.0f)
		{
			slowRate_ = 1.0f;
		}
	}
}

// エネルギー処理
void Player::EnergyProcess()
{
	// ブースト時はエネルギーが減り続ける
	// スローモーション時はブースト時のエネルギーは減らない
	if (isBoost_ && !isSlow_)
	{
		energyGauge_ -= boost_energy_cost;

		// エネルギーがなくなったら通常速度に移行
		if (energyGauge_ <= 0)
		{
			isBoost_ = false;
		}
	}
	// スローモーション時はエネルギーが減り続ける
	else if (isSlow_)
	{
		energyGauge_ -= slow_energy_cost;

		// エネルギーがなくなったら通常に移行
		if (energyGauge_ <= 0)
		{
			isSlow_ = false;
		}
	}
	// 非ブースト時かつ非スローモーション時はエネルギーは回復
	else
	{
		energyGauge_ += energy_recovery_amount;

		// エネルギー上限に達したら回復を止める
		if (energyGauge_ > energy_gauge_total_amount)
		{
			energyGauge_ = energy_gauge_total_amount;
		}
	}
}

// 描画
void Player::Draw()
{
#ifdef _DEBUG
	DrawFormatString(10, 80, 0xffffff, "playerPos = %.2f, %.2f, %.2f", pos_.x, pos_.y, pos_.z);
	DrawFormatString(10, 105, 0xffffff, "energyGauge = %.2f", energyGauge_);
	DrawFormatString(10, 135, 0xffffff, "hp = %d", hp_);
#endif

	// 無敵時間の点滅
	if (IsUltimate())
	{
		if ((ultimateTimer_ / 5) % 2 == 0)
		{
			return;
		}
	}

	// プレイヤーモデルの描画
	if (!isPlayGameOverEffect_)
	{
		pModel_->Draw();
#ifdef _DEBUG
		DrawSphere3D(pos_, model_collision_radius, 8, 0xff0000, 0xff0000, false);
#endif 
	}
}

// プレイヤーの落下処理
void Player::Fall(float fallSpeed)
{
	// 落下
	pos_.y -= (fallSpeed * slowRate_);
}

// プレイヤーのリスポーン処理
void Player::Respawn(VECTOR restartPos)
{
	// Y軸の値を大きくする
	// そのままだと道にめり込むため
	restartPos = VGet(restartPos.x, restartPos.y + 200.0f, restartPos.z);

	// リスポーン
	pos_ = restartPos;
}

// ダメージ処理
void Player::OnDamage()
{
	// 無敵時間中はダメージを受けない
	if (IsUltimate()) return;

	// HPを減らす
	hp_--;

	// 無敵時間の設定
	ultimateTimer_ = ultimate_frames;
}

// プレイヤーの高さが落下死亡判定の高さより小さくなったか
bool Player::IsDeathJudgHeight() const
{
	return (pos_.y < death_judg_height) ? true : false;
}

// プレイヤーが無敵時間中か
bool Player::IsUltimate() const
{
	return (ultimateTimer_ > 0) ? true : false;
}

bool Player::IsLive() const
{
	return (hp_ > 0 ) ? true : false;
}

// 位置情報の取得
VECTOR Player::GetPos() const
{
	return pos_;
}

// ブースト状態かの取得
bool Player::GetIsBoost() const
{
	return isBoost_;
}

// スローモーションのレートの取得
float Player::GetSlowRate() const
{
	return slowRate_;
}

// プレイヤーの当たり判定の半径の取得
float Player::GetCollsionRadius() const
{
	return model_collision_radius;
}

// プレイヤーモデルのハンドルの取得
int Player::GetModelHandle() const
{
	return pModel_->GetModelHandle();
}

// カメラクラスのポインタの設定
void Player::SetCameraPointer(std::shared_ptr<Camera> pCamera)
{
	pCamera_ = pCamera;
}