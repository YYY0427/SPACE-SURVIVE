#pragma once
#include <memory>

class Player;

class Model;

class SkyDome
{
public:
	SkyDome(std::shared_ptr<Player> pPlayer);
	virtual ~SkyDome();

	void Update();
	void Draw();
private:
	std::shared_ptr<Model> pModel_;
	std::shared_ptr<Player> pPlayer_;
};