#pragma once
#include <DxLib.h>
#include <memory>
#include "Util/Range.h"
#include "Util/Timer.h"

/// <summary>
/// カメラの管理クラス
/// </summary>
class Camera
{
public:
	// コンストラクタ
	Camera();

	// デストラクタ
	~Camera();

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="playerPos">プレイヤーの位置座標</param>
	void Update();

	// ゲームクリア時の更新
	void GameClearUpdate(VECTOR playerPos);

	// カメラを決められたフレームの間、x軸に揺らす
	void ShakeX(int shakeFrame, float shakeSize);

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

	// カメラを揺らすフレーム数
	Timer<int> shakeFrame_;

	float cameraVertical_;	// 垂直方向
	float cameraHorizon_;	// 水平方向

	// 回転角度
	float cameraYaw_;	// Y軸
	float cameraPitch_;	// X軸
};