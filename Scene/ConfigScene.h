#pragma once
#include "Scene.h"

// 設定シーン
class ConfigScene : public Scene
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="manager">シーンマネージャーの参照</param>
	ConfigScene(SceneManager& manager);

	// デストラクタ
	virtual ~ConfigScene();

	// 更新
	void Update();

	// 描画
	void Draw();
private:
	// 設定シーンの項目
	enum class Item
	{
		BGM,	
		SE,
		TOTAL_VALUE			// 項目の合計値
	};
private:
	// 現在選択中の項目
	int currentSelectItem_;
};