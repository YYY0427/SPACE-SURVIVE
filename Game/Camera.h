#pragma once
#include <DxLib.h>
#include <memory>

class Player;

/// <summary>
/// カメラの管理クラス
/// </summary>
class Camera
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="pPlayer">プレイヤーのポインタ</param>
	Camera(std::shared_ptr<Player> pPlayer);

	// デストラクタ
	virtual ~Camera();

	// 更新
	void Update();

	// 描画
	void Draw();

	// カメラの回転情報の取得
	float GetCameraYaw();	// Y軸

	// カメラの位置の取得
	VECTOR GetPos();

	// カメラの注視点の取得
	VECTOR GetTarget();
private:
	// ポインタ
	std::shared_ptr<Player> pPlayer_;

	// カメラの位置
	VECTOR cameraPos_;

	// カメラの注視点
	VECTOR cameraTarget_;

	// 視野角
	float perspective_;

	// 回転角度
	float cameraYaw_;	// Y軸
	float cameraPitch_;	// X軸
};