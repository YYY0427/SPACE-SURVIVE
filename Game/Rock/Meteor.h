#pragma once
#include "RockBase.h"
#include "../Util/Effekseer3DEffectManager.h"

class Model;

class Meteor : public RockBase
{
public:
	Meteor(int handle, UnityGameObject rockData);
	virtual ~Meteor();

	void Update();

private:
	int effectDataH_;
};