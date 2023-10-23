#pragma once
#include "RockBase.h"

class Model;

class Rock : public RockBase
{
public:
	Rock(int handle, UnityGameObject rockData);
	virtual ~Rock();

	void Update();

private:
};