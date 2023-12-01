#pragma once
#include "LaserBase.h"
#include "../Util/Timer.h"

class Laser : public LaserBase
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
	Laser(int modelHandle, VECTOR* firePos, VECTOR* vec, float fireFrameCount, bool isInfinity, int chargeFrame);

	// デストラクタ
	~Laser();

	// 更新
	void Update() override;

	// 描画
	void Draw() override;

	// レーザーの反射
	void Stop(const VECTOR pos);

	VECTOR GetVec() const override;

private:
	// 回転行列
	MATRIX rotMtx_;

	// 発射元のベクトルを保存
	VECTOR* vec_;

	// エフェクトの位置
	VECTOR effectPos_;

	// レーザーのエフェクトのハンドル
	int laserEffectHandle_;

	// レーザーの当たり判定モデルとエフェクトの発射フレームの差を測るタイマー
	Timer<int> chargeEffectTimer_;
	
	// レーザーが何フレーム残り続けるかを測るタイマー
	Timer<int> fireFrameTimer_;

	// 発射し続けるか
	bool isInfinity_;

	bool isReflect_;
};

