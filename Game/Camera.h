#pragma once
#include <DxLib.h>
#include <memory>
#include "Util/DataReaderFromUnity.h"
#include "Util/Range.h"

class Player;

/// <summary>
/// カメラの管理クラス
/// </summary>
class Camera
{
public:
	// コンストラクタ
	Camera(std::shared_ptr<Player> pPlayer, UnityGameObject data);

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
	std::shared_ptr<Player> pPlayer_;

	// カメラの位置
	VECTOR cameraPos_;

	// カメラの注視点
	VECTOR cameraTarget_;

	// 視野角
	float perspective_;

	// 視野角の範囲
	Range<float> perspectiveRange_;

	// 回転角度
	float cameraYaw_;	// Y軸
	float cameraPitch_;	// X軸
};