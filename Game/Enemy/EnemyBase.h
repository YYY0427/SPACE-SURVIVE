#pragma once
#include <memory>
#include <DxLib.h>
#include "../Util/Timer.h"
#include "../Util/DataReaderFromUnity.h"

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
	/// �T�C���J�[�u
	/// </summary>
	/// <param name="speed">�T�C���J�[�u���鑬�x</param>
	/// <param name="swingWidth">�U�ꕝ</param>
	void SinWave(const float speed, const float swingWidth);

	virtual void OnDamage(int damage, VECTOR pos);

	bool GetIsEnabled() const;

protected:
	std::unique_ptr<Model> pModel_;
	std::shared_ptr<LazerManager> pLazerManager_;
	std::shared_ptr<Player> pPlayer_;
	VECTOR pos_;
	VECTOR firePos_;
	VECTOR rot_;
	VECTOR moveVec_;
	Timer lazerFireIntervalTimer_;
	int sinWaveTimer_;
	int onDamageEffectHandle_;
	float lazerSpeed_;
	float collisionRadius_;
	float hp_;				
	bool isEnabled_;
};