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
	// 項目
	enum class Item
	{
		DEBUG_SCENE,
		NUM
	};
private:
	// 現在選択中の項目
	int currentSelectItem_;
};

