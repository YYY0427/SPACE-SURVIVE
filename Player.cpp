#include "Player.h"
#include "Camera.h"
#include "Util/InputState.h"
#include "Util/Model.h"
#include <string>

namespace
{
	// プレイヤーモデルのファイルのパス
	string model_file_path = "Data/Model/UFO.mv1";

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

	// ブースト時のエネルギー消費量
	constexpr float boost_energy_cost = 10.0f;

	// エネルギーゲージ
	constexpr float energy_gauge_total_amount = 1000.0f;

	// エネルギーの回復量
	constexpr float energy_recovery_amount = 10.0f;
}

/// <summary>
///  コンストラクタ
/// </summary>
Player::Player() :
	pos_(VGet(0.0f, 0.0f, 0.0f)),
	isInput_(false),
	moveSpeed_(move_normal_speed),
	energyGauge_(energy_gauge_total_amount),
	slowRate_(1.0f)
{
	pModel_ = make_shared<Model>(model_file_path.c_str());
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

	if (InputState::IsTriggered(InputType::SLOW))
	{
		if (slowRate_ == slow_rate)
		{
			slowRate_ = 1.0f;
		}
		else
		{
			slowRate_ = slow_rate;
		}
	}

	// ブースト切り替え
	if (InputState::IsTriggered(InputType::BOOST))
	{
		// 非ブースト時かつエネルギーゲージの残量があった場合ブーストに移行
		if (moveSpeed_ == move_normal_speed && energyGauge_ > 0)
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
	// エネルギーがなくなったら通常速度に移行
	if (moveSpeed_ == move_boost_speed)
	{
		energyGauge_ -= (boost_energy_cost * slowRate_);
		if (energyGauge_ <= 0)
		{
			moveSpeed_ = move_normal_speed;
		}
	}
	// 非ブースト時ならエネルギーは回復
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
	/*if (InputState::IsPressed(InputType::RIZE))
	{
		pos_.y += 3.0f;
	}*/
	if (InputState::IsXInputTrigger(XInputType::RIGHT))
	{
		pos_.y += move_y_speed * slowRate_;
	}
	if (InputState::IsXInputTrigger(XInputType::LEFT))
	{
		pos_.y -= move_y_speed * slowRate_;
	}

	// 位置座標の設定
	pModel_->SetPos(pos_);

	// 向いている方向の設定
	pModel_->SetRot(VGet(moveVec.z * DX_PI_F / 180.0f, 0.0f, -moveVec.x * DX_PI_F / 180.0f));

	// アニメーションを進める
	pModel_->Update();
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
void Player::SetCamera(shared_ptr<Camera> pCamera)
{
	pCamera_ = pCamera;
}