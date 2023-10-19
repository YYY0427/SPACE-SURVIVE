#pragma once
#include "RockBase.h"
#include "../Util/Effekseer3DEffectManager.h"

class Player;
class Model;

class Meteor : public RockBase
{
public:
	Meteor(int handle, std::shared_ptr<Player> pPlayer, UnityGameObject rockData);
	virtual ~Meteor();

	void Update();

private:
	int effectDataH_;
	float effectScale_;
	float effectSpeed_;
	VECTOR effectRot_;

	// ƒ|ƒCƒ“ƒ^
	std::shared_ptr<Player> pPlayer_;
};