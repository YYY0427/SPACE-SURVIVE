#include "Camera.h"
#include "Player.h"
#include "Util/InputState.h"

namespace
{
	// カメラの初期位置
	constexpr VECTOR camera_init_pos = { 0, 200, -500 };

	// カメラの初期注視点
	constexpr VECTOR camera_init_target = { 0, 0, 0 };

	// カメラの回転速度
	constexpr float rot_speed_x = 1.0f;
	constexpr float rot_speed_y = 0.3f;

	// 視野角
	constexpr float normal_perspective = 90.0f;	
	constexpr float boosting_perspective = 120.0f;

	// 描画距離(near, far)
	constexpr float near_distance = 5.0f;
	constexpr float far_distance = 18400.0f;
}

/// <summary>
/// コンストラクタ
/// </summary>
Camera::Camera(Player& pPlayer) :
	cameraPos_(camera_init_pos),
	cameraTarget_(camera_init_target),
	pPlayer_(pPlayer),
	cameraYaw_(0.0f), 
	cameraPitch_(0.0f),
	perspective_(normal_perspective)
{
}

/// <summary>
/// デストラクタ
/// </summary>
Camera::~Camera()
{
}

/// <summary>
/// 更新
/// </summary>
void Camera::Update()
{
	// 右スティックの入力情報の取得
	int up = InputState::IsXInputStick(XInputType::RIGHT, XInputTypeStick::UP);
	int down = InputState::IsXInputStick(XInputType::RIGHT, XInputTypeStick::DOWN);
	int left = InputState::IsXInputStick(XInputType::RIGHT, XInputTypeStick::LEFT);
	int right = InputState::IsXInputStick(XInputType::RIGHT, XInputTypeStick::RIGHT);

	// 入力情報からカメラを回転
	cameraYaw_ += (-left + right) * (rot_speed_x * 0.01f);
	cameraPitch_ += (up + -down) * (rot_speed_y * 0.01f);

	// 縦回転の回転角度の制限
	if (cameraPitch_ >= 60 * DX_PI_F / 180.0f) cameraPitch_ = 60.0f * DX_PI_F / 180.0f;
	if (cameraPitch_ <= -80 * DX_PI_F / 180.0f) cameraPitch_ = -80.0f * DX_PI_F / 180.0f;

	// プレイヤーブースト状態の場合視野角を徐々に大きく
	// ブースト状態の視野角より大きくしない
	if (pPlayer_.GetIsBoost())
	{
		perspective_++;
		if (perspective_ > boosting_perspective)
		{
			perspective_ = boosting_perspective;
		}
	}
	// プレイヤー通常状態の場合視野角を徐々に小さく
	// 通常状態の視野角より小さくはしない
	else
	{
		perspective_--;
		if (perspective_ < normal_perspective)
		{
			perspective_ = normal_perspective;
		}
	}

	// 平行行列の作成
	MATRIX playerTransMtx = MGetTranslate(VScale(pPlayer_.GetPos(), 1.0f));

	// カメラの横回転行列の作成
	MATRIX cameraRotMtxSide = MGetRotY(cameraYaw_);

	// カメラの縦回転行列の作成
	MATRIX cameraRotMtxVertical = MGetRotX(cameraPitch_);

	// カメラの回転行列の作成
	MATRIX cameraRotMtx = MMult(cameraRotMtxVertical, cameraRotMtxSide);

	// カメラの回転行列とプレイヤーの平行行列からカメラの位置行列の作成
	MATRIX cameraMtxPos = MMult(cameraRotMtx, playerTransMtx);

	// カメラの回転行列とプレイヤーの平行行列からカメラの注視点行列の作成
	MATRIX cameraMtxTarget = MMult(cameraRotMtx, playerTransMtx);

	// カメラの位置行列とカメラの初期位置からカメラの位置の作成
	cameraPos_ = VTransform(camera_init_pos, cameraMtxPos);

	// カメラの注視点行列とカメラの初期注視点からカメラの注視点の作成
	cameraTarget_ = VTransform(camera_init_target, cameraMtxTarget);

	// カメラからどれだけ離れたところ( Near )から、 どこまで( Far )のものを描画するかを設定
	SetCameraNearFar(near_distance, far_distance);

	// カメラの視野角を設定(ラジアン)
	SetupCamera_Perspective(perspective_ * DX_PI_F / 180.0f);

	// カメラの位置、どこを見ているかを設定する
	SetCameraPositionAndTargetAndUpVec(cameraPos_, cameraTarget_, VGet(0, 1, 0));
}

/// <summary>
/// 描画
/// </summary>
void Camera::Draw()
{
}

/// <summary>
/// カメラのY軸回転情報の取得
/// </summary>
/// <returns>カメラのY軸回転情報</returns>
float Camera::GetCameraYaw()
{
	return cameraYaw_;
}

/// <summary>
/// 
/// </summary>
/// <returns></returns>
float Camera::GetCameraPitch()
{
	return cameraPitch_;
}

VECTOR Camera::GetPos()
{
	return cameraPos_;
}

VECTOR Camera::GetTarget()
{
	return cameraTarget_;
}
