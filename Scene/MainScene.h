#pragma once
#include "Scene.h"

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

private:
	// メンバ関数ポインタ
	// Updateを切り替えるために作成
	void (MainScene::*updateFunc_) () ;

	int handle_;
};