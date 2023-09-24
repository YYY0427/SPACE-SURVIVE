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
		WHOLE_VOLUME,
		BGM_VOLUME,	
		SE_VOLUME,
		PAD_STICK_SENS_X,
		PAD_STICK_SENS_Y,
		PAD_STICK_REVERSE_X,
		PAD_STICK_REVERSE_Y,
		TOTAL_VALUE			// 項目の合計値
	};
private:
	// 現在選択中の項目
	int currentSelectItem_;
};