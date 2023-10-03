#pragma once
#include <list>
#include <vector>
#include <memory>
#include "../Util/DataReaderFromUnity.h"

class RockBase;
class Player;

class RockManager
{
public:
	RockManager(std::vector<UnityGameObject> rockData, std::vector<UnityGameObject> meteorData, std::shared_ptr<Player> pPlayer);
	virtual ~RockManager();

	void Update();
	void Draw();

	// ‘¶İ‚µ‚Ä‚¢‚é‚©‚ÌŠm”F
	void CheckEnabled();

	std::list<std::shared_ptr<RockBase>> GetRocks();
private:
	std::list<std::shared_ptr<RockBase>> pRocks_;
	int handle_;
	int handle2_;
};