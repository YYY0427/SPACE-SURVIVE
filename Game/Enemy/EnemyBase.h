#pragma once
#include <memory>
#include <vector>
#include <map>
#include <string>
#include <DxLib.h>
#include "../StateMachine.h"
#include "../Util/Timer.h"

class Model;
class LaserManager;
class Player;

enum class EnemyType
{
	NOMAL,
	BOSS,
};

class EnemyBase
{
public:
	EnemyBase();
	virtual ~EnemyBase();

	virtual void InitState() = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;
	virtual void DrawUI(){};

	VECTOR GetPos() const;
	float GetCollisionRadius() const;

	/// <summary>
	/// サインカーブ
	/// </summary>
	/// <param name="speed">サインカーブする速度</param>
	/// <param name="swingWidth">振れ幅</param>
	void SinWave(const float speed, const float swingWidth);

	virtual void OnDamage(int damage, VECTOR pos);

	void Delete();

	bool IsEnabled() const;

protected:
	std::unique_ptr<Model> pModel_;
	std::shared_ptr<LaserManager> pLaserManager_;
	std::shared_ptr<Player> pPlayer_;
	VECTOR pos_;
	VECTOR firePos_;
	VECTOR rot_;
	float opacity_;
	VECTOR moveVec_;
	VECTOR toTargetVec_;

	// 使いまわす汎用タイマー
	std::map<std::string, Timer<int>> utilTimerTable_;

	int sinWaveTimer_;
	int diedEffectHandle_;
	float collisionRadius_;
	float hp_;				
	bool isEnabled_;
};