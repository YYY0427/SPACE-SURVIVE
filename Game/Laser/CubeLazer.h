#pragma once
#include "LazerBase.h"

class CubeLazer : public LazerBase
{
public:
	CubeLazer(int modelHandle, VECTOR pos, VECTOR vec);
	virtual ~CubeLazer();

	void Update() override;
	void Draw() override;
	void CheckInCamera() override;

private:
	float deltaRot_;
	VECTOR endScale_;
};

