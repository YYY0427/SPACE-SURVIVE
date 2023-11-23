#pragma once
#include "LazerBase.h"

class CubeLazer : public LazerBase
{
public:
	CubeLazer(int modelHandle, VECTOR pos, VECTOR vec);
	~CubeLazer();

	void Update() override;
	void Draw() override;
	void ConfirmDelete() override;

private:
	float deltaRot_;
};

