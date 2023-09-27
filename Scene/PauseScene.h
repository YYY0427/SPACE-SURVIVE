#pragma once
#include "Scene.h"

// ポーズシーン
class PauseScene : public Scene
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="manager">シーンマネージャーの参照</param>
	PauseScene(SceneManager& manager);

	// デストラクタ
	virtual ~PauseScene();

	// 更新
	virtual void Update() override;

	// 描画
	void Draw();
private:
	// ポーズシーンから選択できる項目
	enum class Item
	{
		CONTINUE,		// 続ける
		OPTION,			// 設定シーン
		TITLE,			// タイトルシーン
		TOTAL_VALUE		// 項目の合計値
	};
private:
	// 現在選択中の項目
	int currentSelectItem_;
};

