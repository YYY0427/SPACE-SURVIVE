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
	constexpr float normal_perspective = 90.0f;		// 通常時
	constexpr float boosting_perspective = 120.0f;	// ブースト時

	// 描画距離(near, far)
	constexpr float near_distance = 5.0f;
	constexpr float far_distance = 18400.0f;
}

// コンストラクタ
Camera::Camera(std::shared_ptr<Player> pPlayer) :
	cameraPos_(camera_init_pos),
	cameraTarget_(camera_init_target),
	pPlayer_(pPlayer),
	cameraYaw_(0.0f), 
	cameraPitch_(0.0f),
	perspective_(normal_perspective)
{
}

// デストラクタ
Camera::~Camera()
{
}

// 更新
void Camera::Update()
{
	// 右スティックの入力情報の取得
	int up = InputState::IsPadStick(PadLR::RIGHT, PadStickInputType::UP);
	int down = InputState::IsPadStick(PadLR::RIGHT, PadStickInputType::DOWN);
	int left = InputState::IsPadStick(PadLR::RIGHT, PadStickInputType::LEFT);
	int right = InputState::IsPadStick(PadLR::RIGHT, PadStickInputType::RIGHT);

	// 入力情報からカメラを回転
	cameraYaw_ += (-left + right) * (rot_speed_x * 0.01f);
	cameraPitch_ += (up + -down) * (rot_speed_y * 0.01f);

	// 縦回転の回転角度の制限
	if (cameraPitch_ >= 60 * DX_PI_F / 180.0f)
	{
		// 上方向の回転角度の制限
		cameraPitch_ = 60.0f * DX_PI_F / 180.0f;
	}
	if (cameraPitch_ <= -80 * DX_PI_F / 180.0f)
	{
		// 下方向の回転角度の制限
		cameraPitch_ = -80.0f * DX_PI_F / 180.0f;
	}

	// プレイヤーブースト状態の場合視野角を徐々に大きく
	// ブースト状態の視野角より大きくしない
	if (pPlayer_->GetIsBoost())
	{
		perspective_ += 2;
		if (perspective_ > boosting_perspective)
		{
			perspective_ = boosting_perspective;
		}
	}
	// プレイヤー通常状態の場合視野角を徐々に小さく
	// 通常状態の視野角より小さくはしない
	else
	{
		perspective_ -= 2;
		if (perspective_ < normal_perspective)
		{
			perspective_ = normal_perspective;
		}
	}

	// 平行行列の作成
	MATRIX playerTransMtx = MGetTranslate(VScale(pPlayer_->GetPos(), 1.0f));

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

// 描画
void Camera::Draw()
{
}

// カメラのY軸回転情報の取得
float Camera::GetCameraYaw()
{
	return cameraYaw_;
}

// カメラの位置の取得
VECTOR Camera::GetPos()
{
	return cameraPos_;
}

// カメラの注視点の取得
VECTOR Camera::GetTarget()
{
	return cameraTarget_;
}
