#pragma once
#include <memory>
#include <DxLib.h>
#include "../Util/DataReaderFromUnity.h"

class Model;

class RockBase
{
public:
	// コンストラクタ
	RockBase();

	// デストラクタ
	virtual ~RockBase();

	// 更新
	virtual void Update() = 0;

	// 描画
	virtual void Draw();

	// モデルハンドルの取得
	int GetModelHandle() const;

	// 存在するかどうかの取得
	bool GetIsEnabled() const;

protected:
	// ポインタ
	std::shared_ptr<Model> pModel_;

	// 座標
	VECTOR pos_;

	// 回転
	VECTOR rot_;

	// ベクトル
	VECTOR vec_;

	// 存在するか
	bool isEnabled_;
};