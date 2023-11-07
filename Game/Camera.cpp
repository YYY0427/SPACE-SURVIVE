#include "Camera.h"
#include "Player.h"
#include "Util/InputState.h"
#include "Util/SaveData.h"

namespace
{
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
Camera::Camera(std::shared_ptr<Player> pPlayer, UnityGameObject data) :
	cameraPos_(data.pos),
	cameraTarget_(camera_init_target),
	cameraYaw_(0.0f),
	cameraPitch_(0.0f),
	perspective_(normal_perspective),
	perspectiveRange_({ normal_perspective, boosting_perspective }),
	pPlayer_(pPlayer)
{
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

	// カメラの注視点行列とカメラの初期注視点からカメラの注視点の作成
	cameraTarget_ = VTransform(camera_init_target, cameraMtxTarget);
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

	if (InputState::IsPadStick(PadLR::LEFT, PadStickInputType::UP))
	{
//		perspective_ += 1.5f;
	}
	else
	{
//		perspective_ -= 1.5f;
	}
	perspective_ = perspectiveRange_.Clamp(perspective_);

	VECTOR playerPos = pPlayer_->GetPos();
	VECTOR playerVec = pPlayer_->GetMoveVec();

	VECTOR toPlayerVec = VSub(playerPos, cameraPos_);
	toPlayerVec.y = 0.0f;

	if (VSize(toPlayerVec) > 1000.0f && 
		InputState::IsPadStick(PadLR::LEFT, PadStickInputType::UP))
	{
		playerVec.x = 0.0f;
		cameraPos_ = VAdd(cameraPos_, playerVec);
		cameraTarget_ = VAdd(cameraTarget_, playerVec);
	}

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
float Camera::GetCameraYaw() const 
{
	return cameraYaw_;
}

// カメラの位置の取得
VECTOR Camera::GetPos() const 
{
	return cameraPos_;
}

// カメラの注視点の取得
VECTOR Camera::GetTarget() const 
{
	return cameraTarget_;
}
