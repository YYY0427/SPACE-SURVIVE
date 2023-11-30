#pragma once
#include "LaserBase.h"

class CubeLaser : public LaserBase
{
public:
	CubeLaser(int modelHandle, VECTOR pos, VECTOR vec);
	~CubeLaser();

	void Update() override;
	void Draw() override;
	void ConfirmDelete() override;

private:
	float deltaRot_;
	VECTOR endScale_;
};

