#pragma once
#include <memory>
#include <DxLib.h>
#include "../Util/DataReaderFromUnity.h"
#include "../Util/Timer.h"

class Model;
class LazerManager;
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

	virtual void Update() = 0;
	virtual void Draw() = 0;

	VECTOR GetPos() const;
	float GetCollisionRadius() const;

	/// <summary>
	/// サインカーブ
	/// </summary>
	/// <param name="speed">サインカーブする速度</param>
	/// <param name="swingWidth">振れ幅</param>
	void SinWave(const float speed, const float swingWidth);

	void OnDamage(VECTOR pos);

	virtual bool Run();

protected:
	std::unique_ptr<Model> pModel_;
	std::shared_ptr<LazerManager> pLazerManager_;
	std::shared_ptr<Player> pPlayer_;
	VECTOR pos_;
	VECTOR rot_;
	Timer lazerFireIntervalTimer_;
	int sinWaveTimer_;
	int onDamageEffectHandle_;
	float lazerSpeed_;
	float collisionRadius_;
};