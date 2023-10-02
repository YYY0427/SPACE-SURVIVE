#pragma once
#include <list>
#include <memory>
#include <vector>
#include "Util/DataReaderFromUnity.h"

class Enemy;
class Player;

class EnemyManager
{
public:
	EnemyManager(std::vector<DataReaderFromUnity::UnityGameObject> data, std::shared_ptr<Player> pPlayer);
	virtual ~EnemyManager();

	void Init();
	void Update();
	void Draw();

	// ‘¶İ‚µ‚Ä‚¢‚é‚©‚ÌŠm”F
	void CheckEnabled();

	std::list<std::shared_ptr<Enemy>> GetEnemies();
private:
	std::vector<DataReaderFromUnity::UnityGameObject> data_;
	std::shared_ptr<Player> pPlayer_;
	std::list<std::shared_ptr<Enemy>> pEnemies_;
	int handle_;
};