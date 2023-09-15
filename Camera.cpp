#include "Camera.h"
#include <DxLib.h>

namespace
{
	// 視野角
	constexpr float perspective = 60.0f;

	// 描画距離(near, far)
	constexpr float near_distance = 5.0f;
	constexpr float far_distance = 18400.0f;
}

/// <summary>
/// コンストラクタ
/// </summary>
Camera::Camera()
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


	// カメラからどれだけ離れたところ( Near )から、 どこまで( Far )のものを描画するかを設定
	SetCameraNearFar(near_distance, far_distance);

	// カメラの視野角を設定(ラジアン)
	SetupCamera_Perspective(perspective * DX_PI_F / 180.0f);

	// カメラの位置、どこを見ているかを設定する
//	SetCameraPositionAndTarget_UpVecY(cameraPos, cameraTarget);
}

/// <summary>
/// 描画
/// </summary>
void Camera::Draw()
{
}