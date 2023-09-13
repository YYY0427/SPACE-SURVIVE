#pragma once

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

	// 更新
	void Update();

	// 描画
	void Draw();
private:
};