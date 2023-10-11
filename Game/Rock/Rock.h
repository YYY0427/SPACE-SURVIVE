#pragma once
#include "RockBase.h"

class Model;
class Player;

class Rock : public RockBase
{
public:
	Rock(int handle, std::shared_ptr<Player> pPlayer, UnityGameObject rockData);
	virtual ~Rock();

	void Update();

private:
	// �|�C���^
	std::shared_ptr<Player> pPlayer_;
};