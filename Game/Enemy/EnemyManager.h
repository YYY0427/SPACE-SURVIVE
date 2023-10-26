#pragma once
#include <list>
#include <unordered_map>
#include "EnemyBase.h"

enum class EnemyType
{
	NOMAL,
	BOSS,
};

class EnemyManager
{
public:
	EnemyManager();
	virtual ~EnemyManager();

	void Update();
	void Draw();

private:
	std::unordered_map<EnemyType, int> modelHandleTable_;
	std::list<std::shared_ptr<EnemyBase>> pEnemies_;
};