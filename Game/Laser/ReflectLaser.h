#pragma once
#include "LazerBase.h"

class ReflectLaser : public LazerBase
{
public:
	ReflectLaser(int modelHandle, VECTOR pos, VECTOR vec);
	~ReflectLaser();

	void Update() override;
	void Draw() override;

	void ReflectLaserUpdate(VECTOR pos, VECTOR vec) override;

private:
	int laserEffectHandle_;

	MATRIX rotMtx_;
};