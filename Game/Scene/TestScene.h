#pragma once
#include "SceneBase.h"
#include <memory>

// プロトタイプ宣言
class RockManager;
class PlanetManager;
class Camera;
class Player;
class DataReaderFromUnity;
class EnemyManager;
class LazerManager;
class Background;

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

private:
	enum class SceneItem
	{
		PAUSE,
		RESULT, 
		TITLE
	};
private:
	int windEffectH_;
	int backGroundH_;
	SceneItem item_;

	// メンバ関数ポインタ
	// Updateを切り替えるために作成
	void (TestScene::* updateFunc_) ();

	// ポインタ
	std::shared_ptr<Background> pBackground_;
	std::shared_ptr<Camera> pCamera_;
	std::shared_ptr<Player> pPlayer_;
	std::shared_ptr<RockManager> pRockManager_;
	std::shared_ptr<DataReaderFromUnity> pDataReader_;
	std::shared_ptr<PlanetManager> pPlanetManager_;
	std::shared_ptr<EnemyManager> pEnemyManager_;
	std::shared_ptr<LazerManager> pLazerManager_;
};

