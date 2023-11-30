#pragma once
#include <list>
#include <string>
#include <unordered_map>
#include <vector>
#include "EnemyBase.h"

class LaserManager;
class Player;
class Warning;

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
	EnemyManager(std::shared_ptr<Player> pPlayer, std::shared_ptr<LaserManager> pLaserManager);
	~EnemyManager();

	void Update(int time);
	void Draw();
	void DrawUI();

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
	std::shared_ptr<Player> pPlayer_;
	std::shared_ptr<LaserManager> pLaserManager_;
	std::unique_ptr<Warning> pWarning_;

	// モデルハンドルテーブル
	std::unordered_map<EnemyType, int> modelHandleTable_;

	// ボス敵を生成したか
	bool isCreateBossEnemy_;
};