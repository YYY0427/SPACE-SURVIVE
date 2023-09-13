#pragma once
#include "Scene.h"

class InputState;

/// <summary>
/// タイトルシーン
/// </summary>
class TitleScene : public Scene
{
public:
	TitleScene(SceneManager& manager);
	~TitleScene();

	void Update(const InputState& input);
	void Draw();

private:
	// 通常状態のUpdate関数
	void NormalUpdate(const InputState& input);

	// Update用メンバ関数ポインタ
	void (TitleScene::* updateFunc_)(const InputState& input);
};