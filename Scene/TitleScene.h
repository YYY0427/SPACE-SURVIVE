#pragma once
#include "Scene.h"

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

	int currentSelectItem_;
};