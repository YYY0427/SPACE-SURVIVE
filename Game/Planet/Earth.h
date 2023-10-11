#pragma once
#include "PlanetBase.h"

class Earth : public PlanetBase
{
public:
	Earth(int handle, UnityGameObject data);
	virtual ~Earth();

	void Update();

	void Draw();
};