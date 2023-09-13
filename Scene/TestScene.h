#pragma once
#include "Scene.h"

/// <summary>
/// テストシーン(色々試すようのシーン)
/// </summary>
class TestScene : public Scene
{
public:
	// コンストラクタ
	TestScene(SceneManager& manager);

	// デストラクタ
	virtual ~TestScene();

	// 更新
	void Update(const InputState& input);

	// 描画
	void Draw();
private:
};

