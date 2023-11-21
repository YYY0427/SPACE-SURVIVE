#pragma once
#include "EnemyBase.h"
#include <map>
#include <vector>

class HpBar;

class BossEnemy : public EnemyBase
{
public:
	BossEnemy(int modelHandle, std::shared_ptr<Player> pPlayer, std::shared_ptr<LazerManager> pLazerManager);
	virtual ~BossEnemy();

	void Update() override;

	void Draw() override;

	void OnDamage(int damage, VECTOR pos) override;

private:
	void EntryUpdate();
	void IdleUpdate();
	void CubeLaserAttackUpdate();
	void NormalLaserAttackUpdate();
	void MoveUpdate();
	void DiedUpdate();

private:
	enum class MovePosPoint
	{
		CENTER,
		UPPER_LEFT,
		UPPER_RIGHT,
		LOWER_LEFT,
		LOWER_RIGHT,
		TOTAL_VALUE
	};

private:
	// �����o�֐��|�C���^
	// Update��؂�ւ��邽�߂ɍ쐬
	void (BossEnemy::* updateFunc_) ();

	// HP�o�[
	std::unique_ptr<HpBar> pHpBar_;

	std::vector<VECTOR> movePointTable_;

	// �L���[�u���[�U�[�̔��˃C���^�[�o���p�^�C�}�[
	Timer<int> cubeLaserFireIntervalTimer_;

	float cubeLaserSpeed_;

	float opacity_;

	int entryEffectH_;

	VECTOR normalLaserFirePos_;
	bool isNormalLaser_;

	VECTOR goalPos_;
	int movePoint_;
	bool isGoal_;
	bool isMoveUpdateInit_;
};