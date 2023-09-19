#pragma once
#include <DxLib.h>



class Player;

/// <summary>
/// カメラの管理クラス
/// </summary>
class Camera
{
public:
	// コンストラクタ
	Camera(Player& pPlayer_);

	// デストラクタ
	virtual ~Camera();

	// 更新
	void Update();

	// 描画
	void Draw();

	// カメラのY軸回転情報の取得
	float GetCameraYaw();
	float GetCameraPitch();
private:
	Player& pPlayer_;

	VECTOR cameraPos_;
	VECTOR cameraTarget_;

	// 視野角
	float perspective_;

	// 回転角度
	float cameraYaw_;
	float cameraPitch_;
};