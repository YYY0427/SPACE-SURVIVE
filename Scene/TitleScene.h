#pragma once
#include "Scene.h"
#include <array>

// タイトルシーン
class TitleScene : public Scene
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="manager">シーンマネージャーの参照</param>
	TitleScene(SceneManager& manager);

	// デストラクタ
	virtual ~TitleScene();

	// メンバ関数ポインタの更新
	void Update();

	// 描画
	void Draw();

private:
	// 通常状態の更新
	void NormalUpdate();
private:
	struct FadeData
	{
		int fadeValue;
		int fadeSpeed;
		bool isFade;
		bool isGaussFade;
	};
private:
	enum class Item
	{
		START,
		OPSITON,
		END,				
		TOTAL_VALUE			// 項目の合計値
	};
private:
	// メンバ関数ポインタ
	// Updateを切り替えるために作成
	void (TitleScene::* updateFunc_)();

	int handle_;

	std::array<FadeData, static_cast<int>(Item::TOTAL_VALUE)> fadeDataTable_;

	// 現在選択中の項目
	int currentSelectItem_;
};