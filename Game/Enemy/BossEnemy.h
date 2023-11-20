#pragma once
#include "EnemyBase.h"

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
	void CubeLaserAttackUpdate();
	void NormalLaserAttackUpdate();

private:
	// メンバ関数ポインタ
	// Updateを切り替えるために作成
	void (BossEnemy::*updateFunc_) ();

	// HPバー
	std::unique_ptr<HpBar> pHpBar_;		

	Timer<int> cubeLaserFireIntervalTimer_;

	float cubeLaserSpeed_;

	float opacity;

	int entryEffectH_;

	VECTOR toPlayerVec_;

	VECTOR firePos_;
};