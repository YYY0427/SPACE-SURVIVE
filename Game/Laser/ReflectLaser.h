#pragma once
#include "LaserBase.h"

class ReflectLaser : public LaserBase
{
public:
	ReflectLaser(int modelHandle, VECTOR pos, VECTOR vec);
	~ReflectLaser();

	void Update() override;
	void Draw() override;

	void ReflectLaserUpdate(VECTOR pos, VECTOR vec);

private:
	int laserEffectHandle_;

	MATRIX rotMtx_;

	VECTOR goalVec_;
};