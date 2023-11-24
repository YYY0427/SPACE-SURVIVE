#pragma once
#include "EnemyBase.h"
#include "EnemyManager.h"

class NormalEnemy : public EnemyBase
{
public:
	NormalEnemy(
		int modelHandle, 
		std::shared_ptr<Player> pPlayer, 
		std::shared_ptr<LazerManager> pLazerManager,
		VECTOR initPos,
		std::vector<NormalEnemyAIData> normalEnemyGoalPosTable);
	~NormalEnemy();

	void InitState() override {}
	void Update() override;
	void Draw() override;

private:
	void EntarIdle();
	void EntarNormal();
	void EntarShot();
	void EntarDeid();
	void EntarDebug();

	void UpdateIdle();
	void UpdateNormal();
	void UpdateShot();
	void UpdateDeid();
	void UpdateDebug();

	void ExitIdle();
	void ExitNormal();
	void ExitShot();
	void ExitDeid();
	void ExitDebug();

private:
	enum class State
	{
		IDLE,
		NORMAL,
		SHOT,
		DEID,
		DEBUG
	};

private:
	// ステートマシン(ステートを管理する)
	StateMachine<State> stateMachine_;

	std::vector<NormalEnemyAIData> normalEnemyGoalPosTable_;

	VECTOR goalPos_;

	int movePoint_;
};