#pragma once
#include "Scene.h"
#include <array>

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
	struct FadeData
	{
		int fadeValue;
		int fadeSpeed;
		bool isFade;
		bool isGaussFade;
		bool isOnlyStringFade;
	};
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
	std::array<FadeData, static_cast<int>(Item::TOTAL_VALUE)> fadeConfigTable_;

	// 現在選択中の項目
	int currentSelectItem_;
};

