#pragma once
#include "Scene.h"
#include <memory>

class EnemyManager;
class SkyDome;
class Camera;
class Player;

/// <summary>
/// テストシーン(色々試すようのシーン)
/// </summary>
class TestScene : public Scene
{
public:
	// コンストラクタ
	TestScene(SceneManager& manager);

	// デストラクタ
	virtual ~TestScene();

	// 更新
	void Update();

	// 描画
	void Draw();
private:
	// メンバ関数ポインタ
	using UpdateFunc_t = void (TestScene::*) ();
	UpdateFunc_t updateFunc_;

	// 通常の更新
	void NormalUpdate();

	// 
	void GameOverUpdate();

	std::shared_ptr<Camera> pCamera_;
	std::shared_ptr<Player> pPlayer_;
	std::shared_ptr<EnemyManager> pEnemyManager_;
	std::shared_ptr<SkyDome> pSkyDome_;
};

