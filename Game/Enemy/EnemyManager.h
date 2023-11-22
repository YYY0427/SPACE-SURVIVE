#pragma once
#include <list>
#include <unordered_map>
#include <vector>
#include "EnemyBase.h"

class LazerManager;
class Player;
class Warning;

class EnemyManager
{
public:
	EnemyManager(std::shared_ptr<Player> pPlayer, std::shared_ptr<LazerManager> pLazerManager);
	virtual ~EnemyManager();

	void Update(int time);
	void Draw();
	void DrawUI();

	const std::list<std::shared_ptr<EnemyBase>>& GetEnemies() const;	

private:
	void NormalUpdate();
	void CreateBossEnemyUpdate();
	void DeleteNotEnabledEnemy();

private:
	// メンバ関数ポインタ
	// Updateを切り替えるために作成
	void (EnemyManager::*updateFunc_) ();

	std::list<std::shared_ptr<EnemyBase>> pEnemies_;
	std::shared_ptr<Player> pPlayer_;
	std::shared_ptr<LazerManager> pLazerManager_;
	std::unique_ptr<Warning> pWarning_;

	// モデルハンドルテーブル
	std::unordered_map<EnemyType, int> modelHandleTable_;

	// ボス敵を生成したか
	bool isCreateBossEnemy_;
};