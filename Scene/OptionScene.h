#pragma once
#include "Scene.h"
#include <vector>

// 設定シーン
class OptionScene : public Scene
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="manager">シーンマネージャーの参照</param>
	OptionScene(SceneManager& manager);

	// デストラクタ
	virtual ~OptionScene();

	// 更新
	void Update();

	// 描画
	void Draw();
private:
	// 設定シーンの項目
	enum class Item
	{
		LANGUAGE, 
		WINDOW_MODE,
		MASTER_VOLUME,
		BGM_VOLUME,	
		SE_VOLUME,
		PAD_SETTING, 
		BACK,
		/*PAD_STICK_SENS_X,
		PAD_STICK_SENS_Y,
		PAD_STICK_REVERSE_X,
		PAD_STICK_REVERSE_Y,*/
		TOTAL_VALUE			// 項目の合計値
	};

private:
	// 現在選択中の項目
	int currentSelectItem_;

	// 項目の描画色データ
	std::vector<int> itemColorDataTable_;

	// 音量アイコンの画像ハンドル
	int soundIconImgHandle_;
};