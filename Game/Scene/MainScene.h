#pragma once
#include "SceneBase.h"
#include <memory>

// プロトタイプ宣言
class RockManager;
class SkyDome;
class Camera;
class Player;
class DataReaderFromUnity;

// メインシーン
// ゲームのメインの処理を行うシーン
class MainScene : public SceneBase
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
	std::shared_ptr<RockManager> pRockManager_;
	std::shared_ptr<SkyDome> pSkyDome_;
	std::shared_ptr<DataReaderFromUnity> pDataReader_;
};