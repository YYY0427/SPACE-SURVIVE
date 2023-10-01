#pragma once
#include "Scene.h"
#include <memory>

// プロトタイプ宣言
class EnemyManager;
class SkyDome;
class Camera;
class Player;

// メインシーン
// ゲームのメインの処理を行うシーン
class MainScene : public Scene
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="manager">シーンマネージャーの参照</param>
	MainScene(SceneManager& manager);

	// デストラクタ
	virtual ~MainScene();

	// メンバ関数ポインタの更新
	void Update();
	
	// 描画
	void Draw();

private:
	// 通常の更新
	void NormalUpdate();

	// ゲームオーバー時の更新
	void GameOverUpdate();
private:
	// メンバ関数ポインタ
	// Updateを切り替えるために作成
	void (MainScene::*updateFunc_) () ;

	// ポインタ
	std::shared_ptr<Camera> pCamera_;
	std::shared_ptr<Player> pPlayer_;
	std::shared_ptr<EnemyManager> pEnemyManager_;
	std::shared_ptr<SkyDome> pSkyDome_;
};