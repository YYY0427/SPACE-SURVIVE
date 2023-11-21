#pragma once
#include "PlanetBase.h"

class Earth : public PlanetBase
{
public:
	Earth(int handle);
	virtual ~Earth();

	void Update();

	void Draw();
};