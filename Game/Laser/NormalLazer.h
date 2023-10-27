#pragma once
#include "LazerBase.h"

class NormalLazer : public LazerBase
{
public:
	NormalLazer(int modelHandle);
	virtual ~NormalLazer();

	void Update() override;
	void Draw() override;

private:
};

