#pragma once
#include <memory>
#include <DxLib.h>

// プロトタイプ宣言
class Model;

// スカイドーム
class SkyDome
{
public:
	// コンストラクタ
	SkyDome();

	// デストラクタ
	virtual ~SkyDome();

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="playerPos">プレイヤーの位置情報</param>
	void Update(VECTOR playerPos);

	// 描画
	void Draw();
private:
	// モデル
	std::shared_ptr<Model> pModel_;
};