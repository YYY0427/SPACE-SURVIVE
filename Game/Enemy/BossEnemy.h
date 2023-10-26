#pragma once
#include "EnemyBase.h"
#include <DxLib.h>

class BossEnemy : public EnemyBase
{
public:
	BossEnemy(int modelHandle);
	virtual ~BossEnemy();

	void Update() override;
	void Draw() override;

private:
	VECTOR pos_;
	VECTOR rot_;
};