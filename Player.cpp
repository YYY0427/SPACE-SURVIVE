#include "Player.h"
#include "Camera.h"
#include "Util/InputState.h"
#include "Util/Model.h"
#include <string>

namespace
{
	// モデルのファイルのパス
	string model_file_path = "Data/Model/UFO.mv1";

	// プレイヤーの移動量
	constexpr VECTOR player_vec_up{ 0, 0, -1 };
	constexpr VECTOR player_vec_down{ 0, 0, 1 };
	constexpr VECTOR player_vec_right{ 1, 0, 0 };
	constexpr VECTOR player_vec_left{ -1, 0, 0 };

	// プレイヤーの移動速度
	constexpr float player_move_speed = 1.0f;
}

/// <summary>
///  コンストラクタ
/// </summary>
Player::Player() : 
	pos_(VGet(0.0f, 0.0f, 0.0f)),
	isMove_(false)
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

	// 移動情報の初期化
	isMove_ = false;
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
		isMove_ = true;
	}
	if (left > 0)
	{
		moveVecX = VScale(moveLeft, left);
		isMove_ = true;
	}
	if (down > 0)
	{
		moveVecZ = VScale(moveDown, down);
		isMove_ = true;
	}
	if (right > 0)
	{
		moveVecX = VScale(moveRight, right);
		isMove_ = true;
	}
#endif
	// スティックが入力されている場合のみ移動
	if (isMove_)
	{
		// x方向とz方向のベクトルを足して移動ベクトルを作成する
		moveVec = VAdd(moveVecZ, moveVecX);

		// 正規化
	//	moveVec = VNorm(moveVec);

		// 正規化したベクトルにプレイヤーの速度をかける
		moveVec = VScale(moveVec, player_move_speed);

		// 作成した移動ベクトルで座標の移動
		pos_ = VAdd(pos_, moveVec);
	}
	// Y軸の移動
	if (InputState::IsPressed(InputType::RIZE))
	{
		pos_.y += 3.0f;
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
	pModel_->Draw();
	DrawFormatString(10, 80, 0xffffff, "playerPos = %.2f, %.2f, %.2f", pos_.x, pos_.y, pos_.z);
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
/// カメラクラスのポインタのセッター
/// </summary>
/// <param name="pCamera">カメラクラスのポインタ</param>
void Player::SetCamera(shared_ptr<Camera> pCamera)
{
	pCamera_ = pCamera;
}