#pragma once
#include <list>
#include <memory>

class Enemy;
class Player;

class EnemyManager
{
public:
	EnemyManager(std::shared_ptr<Player> pPlayer);
	virtual ~EnemyManager();

	void Update();
	void Draw();

	// ‘¶İ‚µ‚Ä‚¢‚é‚©‚ÌŠm”F
	void CheckEnabled();


	std::list<std::shared_ptr<Enemy>> GetEnemies();
private:
	std::shared_ptr<Player> pPlayer_;
	std::list<std::shared_ptr<Enemy>> pEnemies_;
};