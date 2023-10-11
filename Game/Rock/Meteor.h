#pragma once
#include "RockBase.h"

class Player;
class Model;

class Meteor : public RockBase
{
public:
	Meteor(int handle, std::shared_ptr<Player> pPlayer, UnityGameObject rockData);
	virtual ~Meteor();

	void Update();

private:
	// ƒ|ƒCƒ“ƒ^
	std::shared_ptr<Player> pPlayer_;

	bool isTest_;
};