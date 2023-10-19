#pragma once
#include "SceneBase.h"
#include <array>

// タイトルシーン
class TitleScene : public SceneBase
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
	// フェード設定データ
	struct FadeData
	{
		int fadeValue;
		int fadeSpeed;
		bool isFade;
		bool isGaussFade;
	};
private:
	// 項目
	enum class Item
	{
		START,				// ゲームスタート
		OPSITON,			// オプション
		END,				// ゲーム終了
		TOTAL_VALUE			// 項目の合計値
	};
private:
	// メンバ関数ポインタ
	// Updateを切り替えるために作成
	void (TitleScene::* updateFunc_)();

	// 画像ハンドル
	int handle_;

	// フェード設定のデータテーブル
	std::array<FadeData, static_cast<int>(Item::TOTAL_VALUE)> fadeDataTable_;

	// 現在選択中の項目
	int currentSelectItem_;
};