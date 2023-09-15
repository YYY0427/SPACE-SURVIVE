#pragma once
#include "Scene.h"

/// <summary>
/// タイトルシーン
/// </summary>
class TitleScene : public Scene
{
public:
	// コンストラクタ
	TitleScene(SceneManager& manager);

	// デストラクタ
	virtual ~TitleScene();

	// 更新
	void Update();

	// 描画
	void Draw();
private:
	// 通常状態の更新
	void NormalUpdate();

	// メンバ関数ポインタ
	void (TitleScene::* updateFunc_)();
};