#pragma once
#include "PlanetBase.h"

class Sun : public PlanetBase
{
public:
	Sun(int handle);
	~Sun();

	void Update();

	void Draw();
};