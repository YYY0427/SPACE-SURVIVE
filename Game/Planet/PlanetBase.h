#pragma once
#include <memory>
#include <DxLib.h>
#include "../Util/DataReaderFromUnity.h"

class Model;

class PlanetBase
{
public:
	// コンストラクタ
	PlanetBase();

	// デストラクタ
	virtual ~PlanetBase();

	// 更新
	virtual void Update() = 0;

	// 描画
	virtual void Draw() = 0;

	// モデルハンドルの取得
	int GetModelHandle() const;

protected:
	// ポインタ
	std::shared_ptr<Model> pModel_;

	// 座標
	VECTOR pos_;

	// 回転
	VECTOR rot_;
};