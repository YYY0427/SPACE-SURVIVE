#pragma once
#include "Scene.h"
#include <memory>

using namespace std;

class Camera;

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
	void Update();

	// 描画
	void Draw();
private:
	shared_ptr<Camera> pCamera_;
};

