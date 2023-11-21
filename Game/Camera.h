#pragma once
#include <DxLib.h>
#include <memory>
#include "Util/Range.h"

/// <summary>
/// カメラの管理クラス
/// </summary>
class Camera
{
public:
	// コンストラクタ
	Camera();

	// デストラクタ
	virtual ~Camera();

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="playerPos">プレイヤーの位置座標</param>
	void Update();

	// 描画
	void Draw();

	// カメラの回転情報の取得
	float GetCameraYaw() const;// Y軸

	// カメラの位置の取得
	VECTOR GetPos() const;

	// カメラの注視点の取得
	VECTOR GetTarget() const;

private:
	// カメラの位置
	VECTOR pos_;

	// カメラの注視点
	VECTOR target_;

	// 視野角
	float perspective_;

	// 視野角の範囲
	Range<float> perspectiveRange_;

	// 回転角度
	float cameraYaw_;	// Y軸
	float cameraPitch_;	// X軸
};