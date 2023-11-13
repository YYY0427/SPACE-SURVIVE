#pragma once
#include "EnemyBase.h"

class NormalEnemy : public EnemyBase
{
public:
	NormalEnemy(int modelHandle, std::shared_ptr<Player> pPlayer, std::shared_ptr<LazerManager> pLazerManager, UnityGameObject data);
	virtual ~NormalEnemy();

	void Update() override;
	void Draw() override;

private:
	VECTOR toTargetVec_;
};