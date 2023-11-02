#pragma once
#include "SceneBase.h"
#include <memory>

// プロトタイプ宣言
class RockManager;
class PlanetManager;
class SkyDome;
class Camera;
class Player;
class DataReaderFromUnity;
class RoadManager;
class Road;
class EnemyManager;
class LazerManager;

// テストシーン
// 色々試すようのシーン
class TestScene : public SceneBase
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="manager">シーンマネージャーの参照</param>
	TestScene(SceneManager& manager);

	// デストラクタ
	virtual ~TestScene();

	// メンバ関数ポインタの更新
	void Update();

	// 描画
	void Draw();
private:
	// 通常の更新
	void NormalUpdate();

	// ゲームオーバー時の更新
	void CollisionRockUpdate();

	// プレイヤー落下死亡時の更新
	void DeathFallPlayerUpdate();

	// プレイヤーの落下処理をまとめたもの
	void PlayerFallProcess();

	// 地面の線の描画
	void GroundLineDraw();

	// 道の無限スクロール
	void RoadInfiniteScroll();

	/// <summary>
	/// プレイヤーが道の上にいるか判定
	/// </summary>
	/// <returns>true : 道の上にいる, false : 道の上にいない</returns>
	bool JudgePlayerOnTheRoad();

	// プレイヤーから伸ばした線とその線に当たった道までの距離が特定の距離を超えているか
	bool OverLimitPlayerHeight();

	/// <summary>
	/// プレイヤーから下に伸びている線と道との当たり判定
	/// 当たっている道を見つけたら処理終了
	/// 道はポリゴンを2つ使って描画しているため、resultは2つ
	/// </summary>
	/// <param name="result">線と当たったポリゴン1つ目の当たり判定結果</param>
	/// <param name="result2">線とポリゴン2つ目の当たり判定結果</param>
	void CollisionRoadAndPlayer(std::shared_ptr<Road> pRoad, HITRESULT_LINE& result, HITRESULT_LINE& result2);

	void CollisionAllRoadAndPlayer(HITRESULT_LINE& result, HITRESULT_LINE& result2);
private:
	enum class SceneItem
	{
		PAUSE,
		RESULT, 
		TITLE
	};
private:
	int nextRoad_;
	SceneItem item_;

	// メンバ関数ポインタ
	// Updateを切り替えるために作成
	void (TestScene::* updateFunc_) ();

	// ポインタ
	std::shared_ptr<Camera> pCamera_;
	std::shared_ptr<Player> pPlayer_;
	std::shared_ptr<RockManager> pRockManager_;
	std::shared_ptr<SkyDome> pSkyDome_;
	std::shared_ptr<DataReaderFromUnity> pDataReader_;
	std::shared_ptr<PlanetManager> pPlanetManager_;
	std::shared_ptr<RoadManager> pRoadManager_;
	std::shared_ptr<EnemyManager> pEnemyManager_;
	std::shared_ptr<LazerManager> pLazerManager_;
};

