#include "Camera.h"
#include "Util/InputState.h"
#include "Util/SaveData.h"

namespace
{
	// カメラの初期位置
	constexpr VECTOR camera_init_pos = { 0, 100, -250 };

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
Camera::Camera(UnityGameObject data) :
	cameraPos_(data.pos),
	cameraTarget_(camera_init_target),
	cameraYaw_(0.0f),
	cameraPitch_(0.0f),
	perspective_(normal_perspective),
	perspectiveRange_({ normal_perspective, boosting_perspective })
{
}

// デストラクタ
Camera::~Camera()
{
}

// 更新
void Camera::Update(VECTOR playerPos, VECTOR playerVec)
{
	// 右スティックの入力情報の取得
	int up = InputState::IsPadStick(PadLR::RIGHT, PadStickInputType::UP);
	int down = InputState::IsPadStick(PadLR::RIGHT, PadStickInputType::DOWN);
	int left = InputState::IsPadStick(PadLR::RIGHT, PadStickInputType::LEFT);
	int right = InputState::IsPadStick(PadLR::RIGHT, PadStickInputType::RIGHT);

	// セーブデータの感度情報の取得
	int padStickSensX = SaveData::GetInstance().GetSaveData().padStickSensitivityX;
	int padStickSensY = SaveData::GetInstance().GetSaveData().padStickSensitivityY;

	// セーブデータのリバース情報の取得
	bool padStickReversX = SaveData::GetInstance().GetSaveData().padStickReverseX;
	bool padStickReversY = SaveData::GetInstance().GetSaveData().padStickReverseY;

	int x = 1, y = 1;
	if (padStickReversX)	x *= -1;
	if (padStickReversY)	y *= -1;

	if (InputState::IsPadStick(PadLR::LEFT, PadStickInputType::UP))
	{
		perspective_ += 1.5f;
	}
	else
	{
		perspective_ -= 1.5f;
	}
	perspective_ = perspectiveRange_.Clamp(perspective_);

	// 入力情報からカメラを回転
//	cameraYaw_ += (-left + right) * (padStickSensX * 0.002f) * x;
//	cameraPitch_ += (up + -down) * (padStickSensY * 0.002f) * y;
	
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

	// 平行行列の作成
	MATRIX playerTransMtx = MGetTranslate(VScale(playerPos, 1.0f));

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
//	cameraPos_ = VTransform(camera_init_pos, cameraMtxPos);

	// カメラの注視点行列とカメラの初期注視点からカメラの注視点の作成
//	cameraTarget_ = VTransform(camera_init_target, cameraMtxTarget);

	VECTOR toPlayerVec = VSub(playerPos, cameraPos_);
	toPlayerVec.y = 0.0f;

	if (VSize(toPlayerVec) > 500.0f && 
		InputState::IsPadStick(PadLR::LEFT, PadStickInputType::UP))
	{
		playerVec.x = 0.0f;
		cameraTarget_ = VAdd(cameraTarget_, playerVec);
	}

	static bool isA = false;
	if (!isA)
	{
		// カメラの注視点行列とカメラの初期注視点からカメラの注視点の作成
		cameraTarget_ = VTransform(camera_init_target, cameraMtxTarget);
		isA = true;
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
