#pragma once
#include "LazerBase.h"
#include "../Util/Timer.h"

class NormalLazer : public LazerBase
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="modelHandle">モデルハンドル</param>
	/// <param name="firePos">発射位置</param>
	/// <param name="vec">ベクトル(正規化されてる)</param>
	/// <param name="fireObjectMoveVec">発射元の移動ベクトル</param>
	/// <param name="isContinue">継続発射のレーザーかどうか</param>
	NormalLazer(int modelHandle, VECTOR* firePos, VECTOR* vec, VECTOR* fireObjectMoveVec, bool isContinue);

	// デストラクタ
	~NormalLazer();

	// 更新
	void Update() override;

	// 描画
	void Draw() override;

	// レーザーの反射
//	void Refrect(const VECTOR pos, const VECTOR norm) override;

	// レーザーのエフェクトの再生が終了していたら当たり判定用のモデルを削除
	void ConfirmDelete() override;

private:
	VECTOR effectScale_;

	// 回転行列
	MATRIX rotMtx_;

	// 発射元のベクトルを保存
	VECTOR* vec_;

	// 発射元の移動ベクトルを保存
	VECTOR* fireObjectMoveVec_;

	// 実際に使用するベクトル
	VECTOR actualVec_;

	// エフェクトの位置
	VECTOR effectPos_;

	// レーザーのエフェクトのハンドル
	int laserEffectHandle_;

	// レーザーの当たり判定モデルとエフェクトの発射フレームの差を測るタイマー
	Timer<int> collisionAndEffectDifferenceTimer_;
};

