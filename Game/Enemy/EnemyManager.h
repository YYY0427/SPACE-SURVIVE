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
	EnemyManager(std::shared_ptr<Player> pPlayer, std::shared_ptr<LazerManager> pLazerManager);
	virtual ~EnemyManager();

	void Update();
	void Draw();

	const std::list<std::shared_ptr<EnemyBase>>& GetEnemies() const;	

private:
	// 敵
	std::list<std::shared_ptr<EnemyBase>> pEnemies_;

	// モデルハンドルテーブル
	std::unordered_map<EnemyType, int> modelHandleTable_;
};