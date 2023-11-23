#pragma once
#include "SceneBase.h"
#include <array>
#include <memory>

// デバッグ用シーン
// デバッグをしやすくするために作成
// 各シーンに飛べる
class DebugScene : public SceneBase
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="manager">シーンマネーシャーの参照</param>
	DebugScene(SceneManager& manager);

	// デストラクタ
	~DebugScene();

	// 更新
	void Update() override;

	// 描画
	void Draw() override;

private:
	// デバッグシーンから飛べるシーンの項目
	enum class Item
	{
		GEME_MAIN_SCENE,			// テストシーン
		TITLE_SCENE,		// タイトルシーン
		TEST_SCENE,			// メインシーン
		OPTION_SCENE,		// 設定シーン
		PAUSE_SCENE,		// ポーズシーン
		TOTAL_VALUE			// 項目の合計値
	};
private:
	// 選択中の項目
	int currentSelectItem_;
};