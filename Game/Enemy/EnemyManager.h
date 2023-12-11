#pragma once
#include <list>
#include <string>
#include <unordered_map>
#include <vector>
#include "EnemyBase.h"
#include "../Vector2.h"

class LaserManager;
class Player;
class Warning;
class ScreenEffect;
class UIManager;

struct NormalEnemyActionData
{
	VECTOR goalPos;			// 目的地
	float speed;			// 目的地に向かう速度
	int idleTime;			// 目的地に到達してから次の目的地に向かうまでの待機フレーム
	bool isLaser;			// 目的地に到達したらレーザーを撃つか
	int laserType;			// レーザーを撃つ場合、どのレーザーを撃つか
	int laserIdleFrame;		// レーザーを撃つ場合、目的地に到達してからレーザーを撃つまでの待機フレーム
	int laserFireFrameTime; // レーザーを何フレーム発射し続けるか
	float cubeLaserSpeed;	// キューブレーザーの速度
	int laserChargeFrame;	// レーザーのチャージフレーム
};

class EnemyManager
{
public:
	EnemyManager(std::shared_ptr<Player> pPlayer, std::shared_ptr<LaserManager> pLaserManager, std::shared_ptr<ScreenEffect> pScreenEffect, std::shared_ptr<UIManager> pUIManager);
	~EnemyManager();

	void Update(int time);
	void GameOverUpdate();
	void Draw();
	void DrawUI();

	// ボスの死亡
	bool IsBossDied();

	// 全ての敵の削除
	void DeleteAllEnemy();

	const std::list<std::shared_ptr<EnemyBase>>& GetEnemies() const;	

private:
	void NormalUpdate();
	void CreateBossEnemyUpdate();

	/// <summary>
	/// 雑魚敵の行動ファイルデータのロードと保存
	/// </summary>
	/// <param name="filePath">ファイルパス</param>
	void LoadAndStoreNormalEnemyActionFileData(const std::string filePath);

private:
	// メンバ関数ポインタ
	// Updateを切り替えるために作成
	void (EnemyManager::*updateFunc_) ();

	std::list<std::shared_ptr<EnemyBase>> pEnemies_;
	std::unique_ptr<Warning> pWarning_;

	std::shared_ptr<ScreenEffect> pScreenEffect_;
	std::shared_ptr<Player> pPlayer_;
	std::shared_ptr<LaserManager> pLaserManager_;
	std::shared_ptr<UIManager> pUIManager_;

	// モデルハンドルテーブル
	std::unordered_map<EnemyType, int> modelHandleTable_;

	// ボス敵を生成したか
	bool isCreateBossEnemy_;
};