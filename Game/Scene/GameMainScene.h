#pragma once
#include "SceneBase.h"
#include "../Util/Timer.h"
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

// メインシーン
// ゲームのメインの処理を行うシーン
class GameMainScene : public SceneBase
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="manager">シーンマネージャーの参照</param>
	GameMainScene(SceneManager& manager);

	// デストラクタ
	~GameMainScene();

	// メンバ関数ポインタの更新
	void Update() override;

	// 描画
	void Draw() override;

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
	Timer<int> timer_;
	LONGLONG drawTime_;
	LONGLONG updateTime_;

	// メンバ関数ポインタ
	// Updateを切り替えるために作成
	void (GameMainScene::* updateFunc_) ();

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

