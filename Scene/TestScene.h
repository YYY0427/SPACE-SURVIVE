#pragma once
#include "Scene.h"
#include <memory>

// プロトタイプ宣言
class EnemyManager;
class SkyDome;
class Camera;
class Player;
class DataReaderFromUnity;

// テストシーン
// 色々試すようのシーン
class TestScene : public Scene
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="manager">シーンマネージャーの参照</param>
	TestScene(SceneManager& manager);

	// デストラクタ
	virtual ~TestScene();

	// 初期化
	void Init();

	// メンバ関数ポインタの更新
	void Update();

	// 描画
	void Draw();
private:
	// 通常の更新
	void NormalUpdate();

	// ゲームオーバー時の更新
	void GameOverUpdate();

	// 地面の線の描画
	void GroundLineDraw();
private:
	// メンバ関数ポインタ
	// Updateを切り替えるために作成
	void (TestScene::* updateFunc_) ();

	// ポインタ
	std::shared_ptr<Camera> pCamera_;
	std::shared_ptr<Player> pPlayer_;
	std::shared_ptr<EnemyManager> pEnemyManager_;
	std::shared_ptr<SkyDome> pSkyDome_;
	std::shared_ptr<DataReaderFromUnity> pDataReader_;
};

