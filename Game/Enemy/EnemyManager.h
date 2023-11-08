#pragma once
#include <list>
#include <unordered_map>
#include <vector>
#include "EnemyBase.h"

class LazerManager;
class Player;
class HpBar;

class EnemyManager
{
public:
	EnemyManager(std::shared_ptr<Player> pPlayer, std::shared_ptr<LazerManager> pLazerManager, UnityGameObject bossEnemyData, std::vector<UnityGameObject> normalEnemyData);
	virtual ~EnemyManager();

	void Update();
	void Draw();

	void OnDamage(float damage);

	// 全ての敵が画面外まで逃げたか確認
	void CheckRunAllEnemy();

	// ゲッター
	bool GetIsRepel() const;	// 撃退されたか
	const std::list<std::shared_ptr<EnemyBase>>& GetEnemies() const;	

private:
	// 敵
	std::list<std::shared_ptr<EnemyBase>> pEnemies_;

	// モデルハンドルテーブル
	std::unordered_map<EnemyType, int> modelHandleTable_;

	// HP
	std::unique_ptr<HpBar> pHpBar_;		// HPバー
	float hp_;							// HP

	// 撃退されたか
	bool isRepel_;
};