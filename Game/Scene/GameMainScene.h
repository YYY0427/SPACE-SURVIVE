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
class LaserManager;
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

private:
	enum class SceneItem
	{
		PAUSE,
		RESULT, 
		TITLE
	};

private:
	// シーンアイテム
	SceneItem item_;

	// ゲームクリアフラグ
	bool isGameClear_;

	// ゲーム開始からの経過時間
	Timer<int> gameTimer_;

	// ゲームクリア時に何フレーム間画面を揺らすか
	Timer<int> quakeTimer_;
	
	// ボスの死亡演出のフレーム
	Timer<int> bossDiedEffectFrame_;

	float quakeX_;

	int screenHandle_;

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
	std::shared_ptr<LaserManager> pLaserManager_;
};

