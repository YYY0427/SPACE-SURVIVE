#pragma once
#include "LazerBase.h"

class CubeLazer : public LazerBase
{
public:
	CubeLazer(int modelHandle);
	virtual ~CubeLazer();

	void Update() override;
	void Draw() override;

private:

};

