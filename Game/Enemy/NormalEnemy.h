#pragma once
#include "EnemyBase.h"

class NormalEnemy : public EnemyBase
{
public:
	NormalEnemy(int modelHandle, std::shared_ptr<Player> pPlayer, std::shared_ptr<LazerManager> pLazerManager);
	virtual ~NormalEnemy();

	void InitState() override {}
	void Update() override;
	void Draw() override;

private:
	enum class State
	{

		DEID,
	};

private:
	// ステートマシン(ステートを管理する)
	StateMachine<State> stateMachine_;
};