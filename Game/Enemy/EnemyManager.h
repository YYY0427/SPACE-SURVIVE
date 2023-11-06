#pragma once
#include <list>
#include <unordered_map>
#include <vector>
#include "EnemyBase.h"

class LazerManager;
class Player;

class EnemyManager
{
public:
	EnemyManager(std::shared_ptr<Player> pPlayer, std::shared_ptr<LazerManager> pLazerManager, UnityGameObject bossEnemyData, std::vector<UnityGameObject> normalEnemyData);
	virtual ~EnemyManager();

	void Update();
	void Draw();

	const std::list<std::shared_ptr<EnemyBase>>& GetEnemies() const;
	void OnDamage(int damage);
	bool GetIsRepel() const;
	void CheckRunAllEnemy();
private:
	std::unordered_map<EnemyType, int> modelHandleTable_;
	std::list<std::shared_ptr<EnemyBase>> pEnemies_;
	int hp_;
	bool isRepel_;
};