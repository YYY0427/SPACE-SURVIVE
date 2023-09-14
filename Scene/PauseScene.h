#pragma once
#include "Scene.h"

class InputState;

/// <summary>
/// ポーズシーン
/// </summary>
class PauseScene : public Scene
{
public:
	// コンストラクタ
	PauseScene(SceneManager& manager);

	// デストラクタ
	virtual ~PauseScene();

	// 更新
	virtual void Update(const InputState& input) override;

	// 描画
	void Draw();
private:
};

