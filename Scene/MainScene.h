#pragma once
#include "Scene.h"

/// <summary>
/// ƒƒCƒ“ƒV[ƒ“
/// </summary>
class MainScene : public Scene
{
public:
	MainScene(SceneManager& manager);
	virtual ~MainScene();

	void Update(const InputState& input);
	void Draw();

private:
	using UpdateFunc_t = void (MainScene::*) (const InputState& input);
	UpdateFunc_t updateFunc_ = nullptr;

	void NormalUpdate(const InputState& input);

	int test = -1;
};