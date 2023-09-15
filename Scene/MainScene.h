#pragma once
#include "Scene.h"

/// <summary>
/// メインシーン
/// </summary>
class MainScene : public Scene
{
public:
	// コンストラクタ
	MainScene(SceneManager& manager);

	// デストラクタ
	virtual ~MainScene();

	// 更新
	void Update();

	// 描画
	void Draw();

private:
	// メンバ関数ポインタ
	using UpdateFunc_t = void (MainScene::*) ();
	UpdateFunc_t updateFunc_;

	// 通常の更新
	void NormalUpdate();

private:
};