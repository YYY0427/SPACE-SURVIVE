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
	SkyDome(VECTOR initPos);

	// デストラクタ
	virtual ~SkyDome();

	// 更新
	void Update();

	// 描画
	void Draw();
private:
	// モデル
	std::shared_ptr<Model> pModel_;
};