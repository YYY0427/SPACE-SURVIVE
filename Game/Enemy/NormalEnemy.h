#pragma once
#include "EnemyBase.h"
#include "EnemyManager.h"
#include "../Laser/LaserBase.h"

class NormalEnemy : public EnemyBase
{
public:
	NormalEnemy(
		int modelHandle, 
		std::shared_ptr<Player> pPlayer, 
		std::shared_ptr<LaserManager> pLazerManager,
		VECTOR initPos,
		bool isErase,
		std::vector<NormalEnemyActionData> normalEnemyGoalPosTable);
	~NormalEnemy();

	void InitState() override;
	void Update() override;
	void Draw() override;

private:
	void AllExitFucsion();

	void EntarIdle();
	void EntarNormal();
	void EntarLaser();
	void EntarDeid();
	void EntarDebug();

	void UpdateIdle();
	void UpdateNormal();
	void UpdateLaser();
	void UpdateDeid();
	void UpdateDebug();

	void ExitIdle();
	void ExitNormal();
	void ExitLaser();
	void ExitDeid();
	void ExitDebug();

private:
	enum class State
	{
		IDLE,
		NORMAL,
		LASER,
		DEID,
		DEBUG
	};

private:
	// ステートマシン(ステートを管理する)
	StateMachine<State> stateMachine_;

	std::vector<NormalEnemyActionData> actionDataTable_;

	// 目的地
	VECTOR goalPos_;

	// 最終目的地に到達したら消すか
	bool isErase_;

	int movePoint_;

	bool isGoal_;

	int idleTime_;

	LaserType laserType_;

	int laserFireIdleTime_;

	int laserFireFrameTime_;

	float cubeLaserSpeed_;

	int laserChargeFrame_;
};