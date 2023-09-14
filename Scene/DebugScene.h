#pragma once
#include "Scene.h"

using namespace std;

/// <summary>
/// デバッグ用シーン
/// </summary>
class DebugScene : public Scene
{
public:
	//コンストラクタ
	DebugScene(SceneManager& manager);

	// デストラクタ
	virtual ~DebugScene();

	// 更新
	void Update(const InputState& input);

	// 描画
	void Draw();
private:
	// デバッグシーンから飛べるシーン達
	enum SceneName
	{
		TEST_SCENE,
		TITLE_SCENE,
		MAIN_SCENE,
		SOUNDSETTING_SCENE,
		NUM
	};
private:
	// 現在選択中のインデックス
	int currentSelectIndex_;
};