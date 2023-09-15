#pragma once

/// <summary>
/// プレイヤークラス
/// </summary>
class Player
{
public:
	// コンストラクタ
	Player();

	// デストラクタ
	virtual ~Player();

	// 更新
	void Update();

	// 描画
	void Draw();

private:
};