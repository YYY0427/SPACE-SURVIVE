#include "Player.h"
#include "Camera.h"
#include "Util/InputState.h"
#include "Util/Model.h"
#include "Util//Effekseer3DEffectManager.h"
#include <string>

namespace
{
	// プレイヤーモデルのファイルのパス
	std::string model_file_path = "Data/Model/UFO.mv1";

	// プレイヤーの移動量
	constexpr VECTOR player_vec_up{ 0, 0, -1 };
	constexpr VECTOR player_vec_down{ 0, 0, 1 };
	constexpr VECTOR player_vec_right{ 1, 0, 0 };
	constexpr VECTOR player_vec_left{ -1, 0, 0 };

	// プレイヤーの通常移動速度
	constexpr float move_normal_speed = 1.0f;

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
}

/// <summary>
///  コンストラクタ
/// </summary>
Player::Player() :
	pos_(VGet(0.0f, 2000.0f, 0.0f)),
	isInput_(false),
	moveSpeed_(move_normal_speed),
	energyGauge_(energy_gauge_total_amount),
	slowRate_(1.0f)
{
	pModel_ = std::make_shared<Model>(model_file_path.c_str());
	pModel_->SetScale(VGet(10.0f, 10.0f, 10.0f));
}

/// <summary>
///  デストラクタ
/// </summary>
Player::~Player()
{
}

/// <summary>
/// 更新
/// </summary>
void Player::Update()
{
	// 左スティックの入力情報の取得
	int up = InputState::IsXInputStick(XInputType::LEFT, XInputTypeStick::UP);
	int down = InputState::IsXInputStick(XInputType::LEFT, XInputTypeStick::DOWN);
	int left = InputState::IsXInputStick(XInputType::LEFT, XInputTypeStick::LEFT);
	int right = InputState::IsXInputStick(XInputType::LEFT, XInputTypeStick::RIGHT);

	// プレイヤーの回転値を取得する
	VECTOR vect = MV1GetRotationXYZ(pModel_->GetModelHandle());

	// カメラが向いている方向からベクトル変換して移動情報作成
	VECTOR moveUp = VTransform(player_vec_up, MGetRotY(pCamera_->GetCameraYaw()));
	VECTOR moveDown = VTransform(player_vec_down, MGetRotY(pCamera_->GetCameraYaw()));
	VECTOR moveRight = VTransform(player_vec_right, MGetRotY(pCamera_->GetCameraYaw() + vect.x));
	VECTOR moveLeft = VTransform(player_vec_left, MGetRotY(pCamera_->GetCameraYaw() + vect.x));

	// スローモーション切り替え
	if (InputState::IsTriggered(InputType::SLOW))
	{
		// スローモーションじゃないかつエネルギーゲージの残量があったらスローモーションに移行
		if (slowRate_ == 1.0f && energyGauge_ >0)
		{
			slowRate_ = slow_rate;
		}
		// スローモーション時は通常に移行
		else
		{
			slowRate_ = 1.0f;
		}
	}
	// ブースト切り替え
	if (InputState::IsTriggered(InputType::BOOST))
	{
		// 非ブースト時かつスローモーションじゃないかつエネルギーゲージの残量があった場合ブーストに移行
		if (moveSpeed_ == move_normal_speed && energyGauge_ > 0 && slowRate_ != slow_rate)
		{
			moveSpeed_ = move_boost_speed;
		}
		// ブースト時の場合は通常速度に移行
		else
		{
			moveSpeed_ = move_normal_speed;
		}
	}
	// ブースト時はエネルギーが減り続ける
	// スローモーション時はブースト時のエネルギーは減らない
	if (moveSpeed_ == move_boost_speed && slowRate_ != slow_rate)
	{
		energyGauge_ -= (boost_energy_cost);

		// エネルギーがなくなったら通常速度に移行
		if (energyGauge_ <= 0)
		{
			moveSpeed_ = move_normal_speed;
		}
	}
	// スローモーション時はエネルギーが減り続ける
	else if (slowRate_ == slow_rate)
	{
		energyGauge_ -= (slow_energy_cost);

		// エネルギーがなくなったら通常に移行
		if (energyGauge_ <= 0)
		{
			slowRate_ = 1.0f;
		}
	}
	// 非ブースト時かつ非スローモーション時はエネルギーは回復
	else
	{
		energyGauge_ += (energy_recovery_amount * slowRate_);
		if (energyGauge_ > energy_gauge_total_amount)
		{
			energyGauge_ = energy_gauge_total_amount;
		}
	}

	// 移動情報の初期化
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
	// スティックが入力されていたら移動ベクトルにスティックが傾いている方向のベクトルを代入
	// スティックの傾きぐわいによってベクトルが大きくなる
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
	// スティックが入力されている場合のみ移動
	if (isInput_)
	{
		// x方向とz方向のベクトルを足して移動ベクトルを作成する
		moveVec = VAdd(moveVecZ, moveVecX);

		// 正規化
	//	moveVec = VNorm(moveVec);

		// 正規化したベクトルにプレイヤーの速度をかける
		moveVec = VScale(moveVec, moveSpeed_);

		moveVec = VScale(moveVec, slowRate_);

		// 作成した移動ベクトルで座標の移動
		pos_ = VAdd(pos_, moveVec);
	}
	// Y軸の移動
	if (InputState::IsXInputTrigger(XInputType::RIGHT))
	{
		pos_.y += move_y_speed * moveSpeed_ * slowRate_;
	}
	if (InputState::IsXInputTrigger(XInputType::LEFT))
	{
		pos_.y -= move_y_speed * moveSpeed_ * slowRate_;
	}

	// 位置座標の設定
	pModel_->SetPos(pos_);

	// 向いている方向の設定
	pModel_->SetRot(VGet(moveVec.z * DX_PI_F / 180.0f, 0.0f, -moveVec.x * DX_PI_F / 180.0f));

	// アニメーションを進める
	pModel_->Update();
}

bool Player::GameOverUpdate()
{
	static bool isPass = false;
	if (!isPass)
	{
		isPass = true;
		Effekseer3DEffectManager::GetInstance().PlayEffect("explosion", pos_, VGet(20.0f, 20.0f, 20.0f), 1.0f);
	}

	// 位置座標の設定
	pModel_->SetPos(pos_);

	// アニメーションを進める
	pModel_->Update();

	if (!Effekseer3DEffectManager::GetInstance().IsPlayingEffect("explosion"))
	{
		return true;
	}
	return false;
}

/// <summary>
/// 描画
/// </summary>
void Player::Draw()
{
	// プレイヤーモデルの描画
	pModel_->Draw();
#ifdef _DEBUG
	DrawFormatString(10, 80, 0xffffff, "playerPos = %.2f, %.2f, %.2f", pos_.x, pos_.y, pos_.z);
	DrawFormatString(10, 105, 0xffffff, "energyGauge = %.2f", energyGauge_);
#endif
}

/// <summary>
/// 位置情報の取得
/// </summary>
/// <returns>位置情報</returns>
VECTOR Player::GetPos()
{
	return pos_;
}

/// <summary>
/// ブースト状態かの取得
/// </summary>
/// <returns>true : ブースト状態、false : 通常状態</returns>
bool Player::GetIsBoost()
{
	if (moveSpeed_ == move_boost_speed)
	{
		return true;
	}
	return false;
}

/// <summary>
/// スローモーションのレートの取得
/// </summary>
/// <returns>スローモーションのレート</returns>
float Player::GetSlowRate()
{
	return slowRate_;
}

/// <summary>
/// カメラクラスのポインタのセッター
/// </summary>
/// <param name="pCamera">カメラクラスのポインタ</param>
void Player::SetCamera(std::shared_ptr<Camera> pCamera)
{
	pCamera_ = pCamera;
}