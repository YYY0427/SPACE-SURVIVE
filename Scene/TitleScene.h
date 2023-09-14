#pragma once
#include "Scene.h"

class InputState;

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
	void Update(const InputState& input);

	// 描画
	void Draw();
private:
	// 通常状態の更新
	void NormalUpdate(const InputState& input);

	// メンバ関数ポインタ
	void (TitleScene::* updateFunc_)(const InputState& input);
};