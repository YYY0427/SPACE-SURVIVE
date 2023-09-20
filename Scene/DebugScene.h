#pragma once
#include "Scene.h"



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
	void Update();

	// 描画
	void Draw();
private:
	// デバッグシーンから飛べるシーンの項目
	enum class SceneItem
	{
		TEST_SCENE,
		TITLE_SCENE,
		MAIN_SCENE,
		SOUNDSETTING_SCENE,
		PAUSE_SCENE,
		NUM
	};
private:
	// 現在選択中の項目
	int currentSelectItem_;
};