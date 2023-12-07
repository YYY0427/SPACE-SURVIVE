#include "Camera.h"
#include "Player.h"
#include "Util/InputState.h"
#include "Util/SaveData.h"
#include "Util/MathUtil.h"

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

	// プレイヤーからの距離
	constexpr float camera_distance = 300.0f;

	// ゲームクリア時のカメラの回転速度
	constexpr float camera_rotate_speed = 0.03f;

	// ゲームクリア時のカメラの移動速度
	constexpr float camera_move_speed = 3.0f;

}

// コンストラクタ
Camera::Camera() :
	cameraYaw_(0.0f),
	cameraPitch_(0.0f),
	perspective_(normal_perspective),
	perspectiveRange_({ normal_perspective, boosting_perspective }),
	cameraVertical_(0.0f),
	cameraHorizon_(DX_PI_F),
	isFirst_(false),
	shakeFrame_(0),
	goalTarget_({}),
	goalPos_({})
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
	SetupCamera_Perspective(MathUtil::DegreeFromRadian(perspective_));

	// カメラの位置、どこを見ているかを設定する
	SetCameraPositionAndTargetAndUpVec(pos_, target_, VGet(0, 1, 0));
}

// ゲームクリア時の更新
void Camera::GameClearUpdate(VECTOR playerPos)
{
	if (!isFirst_)
	{
		isFirst_ = true;
		goalTarget_ = playerPos;
	}

	cameraHorizon_ -= camera_rotate_speed;
	cameraHorizon_ = (std::max)(cameraHorizon_, MathUtil::DegreeFromRadian(30));

	// 基準の長さを垂直方向に回転させたときの水平分の長さ
	float verticalLength = camera_distance * cosf(cameraVertical_);

	// 高さ
	float horizonLength = camera_distance * sinf(cameraVertical_);

	// カメラ座標の設定
	// xz座標は水平方向の長さ分進めたところ
	goalPos_.x = playerPos.x + verticalLength * sinf(cameraHorizon_);
	goalPos_.z = playerPos.z + verticalLength * cosf(cameraHorizon_);

	// Ｙ座標は垂直方向分上に
	goalPos_.y = playerPos.y + horizonLength;

	if (VSize(VSub(goalPos_, pos_)) > camera_move_speed)
	{
		VECTOR moveVec = VScale(VNorm(VSub(goalPos_, pos_)), camera_move_speed);
		pos_ = VAdd(pos_, moveVec);
	}

	if (VSize(VSub(goalTarget_, target_)) > camera_move_speed)
	{
		VECTOR targetVec = VScale(VNorm(VSub(goalTarget_, target_)), camera_move_speed);
		target_ = VAdd(target_, targetVec);
	}
}

void Camera::GameOverUpdate(VECTOR playerPos)
{
	goalTarget_ = playerPos;

	if (VSize(VSub(goalTarget_, target_)) > camera_move_speed)
	{
		VECTOR targetVec = VScale(VNorm(VSub(goalTarget_, target_)), camera_move_speed);
		target_ = VAdd(target_, targetVec);
	}
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