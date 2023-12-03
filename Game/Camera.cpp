#include "Camera.h"
#include "Player.h"
#include "Util/InputState.h"
#include "Util/SaveData.h"

namespace
{
	// カメラの初期位置
	constexpr VECTOR camera_pos = { 0, 0, -300 };

	// カメラの初期注視点
	constexpr VECTOR camera_init_target = { 0, 0, 0 };

	// 視野角
	constexpr float normal_perspective = 90.0f;		// 通常時
	constexpr float boosting_perspective = 120.0f;	// ブースト時

	// 描画距離(near, far)
	constexpr float near_distance = 5.0f;
	constexpr float far_distance = 20000.0f;
}

// コンストラクタ
Camera::Camera() :
	cameraYaw_(0.0f),
	cameraPitch_(0.0f),
	perspective_(normal_perspective),
	perspectiveRange_({ normal_perspective, boosting_perspective })
{
	pos_ = camera_pos;
	target_ = camera_init_target;
}

// デストラクタ
Camera::~Camera()
{
}

// 更新
void Camera::Update()
{
	// カメラからどれだけ離れたところ( Near )から、 どこまで( Far )のものを描画するかを設定
	SetCameraNearFar(near_distance, far_distance);

	// カメラの視野角を設定(ラジアン)
	SetupCamera_Perspective(perspective_ * DX_PI_F / 180.0f);

	// カメラの位置、どこを見ているかを設定する
	SetCameraPositionAndTargetAndUpVec(pos_, target_, VGet(0, 1, 0));
}

void Camera::ShakeX(int shakeFrame, float shakeSize)
{
	shakeFrame_ = shakeFrame;
	shakeFrame_.Update(1);
	if (!shakeFrame_.IsTimeOut())
	{
		pos_.x += sinf(GetNowCount() / 10.0f) * shakeSize;
		shakeSize *= 0.96f;
	}
}

// 描画
void Camera::Draw()
{
}

// カメラのY軸回転情報の取得
float Camera::GetCameraYaw() const 
{
	return cameraYaw_;
}

// カメラの位置の取得
VECTOR Camera::GetPos() const 
{
	return pos_;
}

// カメラの注視点の取得
VECTOR Camera::GetTarget() const 
{
	return target_;
}
