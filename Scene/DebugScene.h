#pragma once
#include "Scene.h"
#include <array>
#include <memory>

// デバッグ用シーン
// デバッグをしやすくするために作成
// 各シーンに飛べる
class DebugScene : public Scene
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="manager">シーンマネーシャーの参照</param>
	DebugScene(SceneManager& manager);

	// デストラクタ
	virtual ~DebugScene();

	// 更新
	void Update();

	// 描画
	void Draw();
private:
	struct FadeData
	{
	};
private:
	// デバッグシーンから飛べるシーンの項目
	enum class Item
	{
		TEST_SCENE,			// テストシーン
		TITLE_SCENE,		// タイトルシーン
		MAIN_SCENE,			// メインシーン
		OPTION_SCENE,		// 設定シーン
		PAUSE_SCENE,		// ポーズシーン
		TOTAL_VALUE			// 項目の合計値
	};
private:
	// 現在選択中の項目
	int currentSelectItem_;
};