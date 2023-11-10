#pragma once
#include "LazerBase.h"

class CubeLazer : public LazerBase
{
public:
	CubeLazer(int modelHandle, VECTOR pos, VECTOR vec);
	virtual ~CubeLazer();

	void Update(VECTOR scrollVec) override;
	void Draw() override;
	void CheckInCamera() override;

private:
	float deltaRot_;
};

