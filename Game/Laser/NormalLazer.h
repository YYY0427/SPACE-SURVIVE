#pragma once
#include "LazerBase.h"
#include "../Util/Timer.h"

class NormalLazer : public LazerBase
{
public:
	NormalLazer(int modelHandle, VECTOR pos, VECTOR vec);
	virtual ~NormalLazer();

	void Update() override;
	void Draw() override;

	void Fire(const VECTOR pos, const VECTOR vec);
	void Refrect(const VECTOR pos, const VECTOR norm) override;
	void CheckInCamera() override;

private:
	int lazerEffectHandle_;

	// レーザーの当たり判定モデルとエフェクトの発射フレームの差を測るタイマー
	Timer collisionAndEffectDifferenceTimer_;
};

