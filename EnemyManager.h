#pragma once
#include <vector>
#include <memory>

class Enemy;
class Player;

class EnemyManager
{
public:
	EnemyManager(Player& pPlayer);
	virtual ~EnemyManager();

	void Update();
	void Draw();

	// 存在しているかの確認
	void CheckEnabled();


	std::vector<std::shared_ptr<Enemy>> GetEnemies();
private:
	Player& pPlayer_;
	std::vector<std::shared_ptr<Enemy>> pEnemies_;
};