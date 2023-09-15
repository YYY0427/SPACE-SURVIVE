#pragma once
#include "Scene.h"

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
	virtual void Update() override;

	// 描画
	void Draw();
private:
};

