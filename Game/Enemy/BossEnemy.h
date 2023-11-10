#pragma once
#include "EnemyBase.h"

class HpBar;

class BossEnemy : public EnemyBase
{
public:
	BossEnemy(int modelHandle, std::shared_ptr<Player> pPlayer, std::shared_ptr<LazerManager> pLazerManager, UnityGameObject data);
	virtual ~BossEnemy();

	void Update() override;
	void Draw() override;

	void OnDamage(int damage, VECTOR pos) override;
private:
	// HP
	std::unique_ptr<HpBar> pHpBar_;		// HPÉoÅ[
};