#pragma once
#include "Scene.h"
#include <memory>

// プロトタイプ宣言
class RockManager;
class PlanetManager;
class SkyDome;
class Camera;
class Player;
class DataReaderFromUnity;
class Imge3D;

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
	std::shared_ptr<RockManager> pRockManager_;
	std::shared_ptr<SkyDome> pSkyDome_;
	std::shared_ptr<DataReaderFromUnity> pDataReader_;
	std::shared_ptr<PlanetManager> pPlanetManager_;
	std::shared_ptr<Imge3D> img3D_;
	int handle_;
};

