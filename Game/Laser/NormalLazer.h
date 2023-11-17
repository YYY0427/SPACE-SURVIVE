#pragma once
#include "LazerBase.h"
#include "../Util/Timer.h"

class NormalLazer : public LazerBase
{
public:
	NormalLazer(int modelHandle, VECTOR* pos, VECTOR* vec, VECTOR* enemyMoveVec);
	virtual ~NormalLazer();

	void Update() override;
	void Draw() override;

	void Refrect(const VECTOR pos, const VECTOR norm) override;
	void CheckInCamera() override;

private:
	VECTOR* vec_;
	VECTOR* enemyMoveVec_;
	VECTOR a_;
	VECTOR effectPos_;
	int lazerEffectHandle_;

	// レーザーの当たり判定モデルとエフェクトの発射フレームの差を測るタイマー
	Timer<int> collisionAndEffectDifferenceTimer_;
};

