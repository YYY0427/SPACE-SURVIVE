#include "OptionScene.h"
#include "SceneManager.h"
#include "DebugScene.h"
#include "../Util/InputState.h"
#include "../Util/SoundManager.h"
#include "../Util/SaveData.h"
#include "../Util/StringManager.h"
#include "../Util/DrawFunctions.h"
#include "../common.h"
#include <DxLib.h>
#include <array>

namespace
{
	// 表示するテキストの全体の位置
	constexpr int draw_text_pos_x = 350;
	constexpr int draw_text_pos_y = common::screen_height / 2 - 150;

	// テキストの文字間
	constexpr int text_space_y = 64;

	// 選択されていないときの描画色
	constexpr int normal_color = 0x444444;

	// 選択されているときの描画色
	constexpr int choose_color = 0xffffff;
}

// コンストラクタ
OptionScene::OptionScene(SceneManager& manager) :
	Scene(manager),
	currentSelectItem_(0),
	soundIconImgHandle_(-1)
{
	// 画像のロード
	soundIconImgHandle_ = my::MyLoadGraph("Data/Image/Sound.png");

	// 項目の描画色を選択されていないときの色に初期化
	for (int i = 0; i < static_cast<int>(Item::TOTAL_VALUE); i++)
	{
		itemColorDataTable_.push_back(normal_color);
	}
}

// デストラクタ
OptionScene::~OptionScene()
{
	DeleteGraph(soundIconImgHandle_);
}

// 更新
void OptionScene::Update()
{
	// カラーの初期化
	for (auto& itemColor : itemColorDataTable_)
	{
		itemColor = normal_color;
	}

	// 選択肢を回す処理
	int itemTotalValue = static_cast<int>(Item::TOTAL_VALUE);
	if (InputState::IsTriggered(InputType::UP))
	{
		currentSelectItem_ = ((currentSelectItem_ - 1) + itemTotalValue) % itemTotalValue;
	}
	else if (InputState::IsTriggered(InputType::DOWN))
	{
		currentSelectItem_ = (currentSelectItem_ + 1) % itemTotalValue;
	}

	// 選択されている項目の色を変える
	itemColorDataTable_[currentSelectItem_] = choose_color;

	switch (currentSelectItem_)
	{
	case static_cast<int>(Item::LANGUAGE):
		break;
	case static_cast<int>(Item::WINDOW_MODE):
		break;
	case static_cast<int>(Item::MASTER_VOLUME):
		SaveData::GetInstance().SetMasterVolume();
		break;
	case static_cast<int>(Item::BGM_VOLUME):
		SaveData::GetInstance().SetBgmVolume();
		break;
	case static_cast<int>(Item::SE_VOLUME):
		SaveData::GetInstance().SetSeVolume();
		break;
	case static_cast<int>(Item::PAD_SETTING):
		break;
	case static_cast<int>(Item::BACK):
		break;
	}
	//else if (currentSelectItem_ == static_cast<int>(Item::PAD_STICK_SENS_X))
	//{
	//	// 感度の最大値より大きくなったら最小値にする
	//	SaveData::GetInstance().SetPadStickSensitivityX();
	//}
	//else if (currentSelectItem_ == static_cast<int>(Item::PAD_STICK_SENS_Y))
	//{
	//	// 感度の最大値より大きくなったら最小値にする
	//	SaveData::GetInstance().SetPadStickSensitivityY();
	//}
	//else if (currentSelectItem_ == static_cast<int>(Item::PAD_STICK_REVERSE_X))
	//{
	//	SaveData::GetInstance().SetPadStickReverseX();
	//}
	//else if (currentSelectItem_ == static_cast<int>(Item::PAD_STICK_REVERSE_Y))
	//{
	//	SaveData::GetInstance().SetPadStickReverseY();
	//}

	if (InputState::IsTriggered(InputType::BACK))
	{
		manager_.PopScene();
		return;
	}
}

// 描画
void OptionScene::Draw()
{
	// 現在のシーンのテキスト表示
	DrawString(0, 0, "ConfigScene", 0xffffff, true);

	auto& stringManager = StringManager::GetInstance();

	// シーンタイトルの描画
	stringManager.DrawStringCenter("OptionTitle", common::screen_width / 2, 100, 0xffffff);

	// 項目の描画
	int language = static_cast<int>(Item::LANGUAGE);
	int windowMode = static_cast<int>(Item::WINDOW_MODE);
	int masterVolume = static_cast<int>(Item::MASTER_VOLUME);
	int bgmVolume = static_cast<int>(Item::BGM_VOLUME);
	int seVolume = static_cast<int>(Item::SE_VOLUME);
	int padSetting = static_cast<int>(Item::PAD_SETTING);
	int back = static_cast<int>(Item::BACK);
	stringManager.DrawStringCenter("OptionItemLanguage", draw_text_pos_x, draw_text_pos_y + text_space_y * language, itemColorDataTable_[language]);
	stringManager.DrawStringCenter("OptionItemWindowMode", draw_text_pos_x, draw_text_pos_y + text_space_y * windowMode, itemColorDataTable_[windowMode]);
	stringManager.DrawStringCenter("OptionItemMasterVolume", draw_text_pos_x, draw_text_pos_y + text_space_y * masterVolume, itemColorDataTable_[masterVolume]);
	stringManager.DrawStringCenter("OptionItemBgmVolume", draw_text_pos_x, draw_text_pos_y + text_space_y * bgmVolume, itemColorDataTable_[bgmVolume]);
	stringManager.DrawStringCenter("OptionItemSeVolume", draw_text_pos_x, draw_text_pos_y + text_space_y * seVolume, itemColorDataTable_[seVolume]);
	stringManager.DrawStringCenter("OptionItemPadSetting", common::screen_width / 2, draw_text_pos_y + text_space_y * padSetting, itemColorDataTable_[padSetting]);
	stringManager.DrawStringCenter("OptionItemBack", common::screen_width / 2, draw_text_pos_y + text_space_y * back, itemColorDataTable_[back]);

	// 各サウンド音量の表示
	std::array<int, 3> volume;
	volume[0] = SaveData::GetInstance().GetSaveData().masterVolume;
	volume[1] = SaveData::GetInstance().GetSaveData().bgmVolume;
	volume[2] = SaveData::GetInstance().GetSaveData().seVolume;
	int item = masterVolume;
	for(int i = 0; i < volume.size(); i++)
	{
		for (int j = 0; j < volume[i]; j++)
		{
			SetDrawBright(70, 70, 70);
			if (currentSelectItem_ == item)
			{
				SetDrawBright(255, 255, 255);
			}
			int textSpaceX = j * 70;
			DrawRotaGraph(draw_text_pos_x + 170 + textSpaceX, draw_text_pos_y + text_space_y * item + 10, 0.2, 0.0, soundIconImgHandle_, true);
		}
		item++;
	}
	SetDrawBright(255, 255, 255);

	//// BGMの項目と音量の表示
	//int whole = static_cast<int>(Item::MASTER_VOLUME);
	//DrawString(draw_text_pos_x, draw_text_pos_y + text_space_y * whole, "全体音量", 0xffffff, true);
	//DrawFormatString(draw_text_pos_x + text_space_y + 50, draw_text_pos_y + text_space_y * whole, 0xffffff, "%d", SaveData::GetInstance().GetSaveData().masterVolume);

	//// BGMの項目と音量の表示
	//int bgm = static_cast<int>(Item::BGM_VOLUME);
	//DrawString(draw_text_pos_x, draw_text_pos_y + text_space_y * bgm, "BGM", 0xffffff, true);
	//DrawFormatString(draw_text_pos_x + text_space_y, draw_text_pos_y + text_space_y * bgm, 0xffffff, "%d", SaveData::GetInstance().GetBgmVolume());

	//// SEの項目と音量の表示
	//int se = static_cast<int>(Item::SE_VOLUME);
	//DrawString(draw_text_pos_x, draw_text_pos_y + text_space_y * se, "SE", 0xffffff, true);
	//DrawFormatString(draw_text_pos_x + text_space_y, draw_text_pos_y + text_space_y * se, 0xffffff, "%d", SaveData::GetInstance().GetSeVolume());

	//// スティックの感度Xの項目と音量の表示
	//int padStickX = static_cast<int>(Item::PAD_STICK_SENS_X);
	//DrawString(draw_text_pos_x, draw_text_pos_y + text_space_y * padStickX, "パッドの横感度", 0xffffff, true);
	//DrawFormatString(draw_text_pos_x + text_space_y + 100, draw_text_pos_y + text_space_y * padStickX, 0xffffff, "%d", SaveData::GetInstance().GetPadStickSensitivityX());

	//// スティックの感度Yの項目と音量の表示
	//int padStickY = static_cast<int>(Item::PAD_STICK_SENS_Y);
	//DrawString(draw_text_pos_x, draw_text_pos_y + text_space_y * padStickY, "パッドの縦感度", 0xffffff, true);
	//DrawFormatString(draw_text_pos_x + text_space_y + 100, draw_text_pos_y + text_space_y * padStickY, 0xffffff, "%d", SaveData::GetInstance().GetPadStickSensitivityY());

	//int padStickReverseX = static_cast<int>(Item::PAD_STICK_REVERSE_X);
	//DrawString(draw_text_pos_x, draw_text_pos_y + text_space_y * padStickReverseX, "パッドの横リバース", 0xffffff, true);
	//if (!SaveData::GetInstance().GetPadStickReverseX())
	//{
	//	DrawString(draw_text_pos_x + text_space_y + 100, draw_text_pos_y + text_space_y * padStickReverseX, "NORMAL", 0xffffff);
	//}
	//else
	//{
	//	DrawString(draw_text_pos_x + text_space_y + 100, draw_text_pos_y + text_space_y * padStickReverseX, "REVERSE", 0xffffff);
	//}

	//int padStickReverseY = static_cast<int>(Item::PAD_STICK_REVERSE_Y);
	//DrawString(draw_text_pos_x, draw_text_pos_y + text_space_y * padStickReverseY, "パッドの縦リバース", 0xffffff, true);
	//if (!SaveData::GetInstance().GetPadStickReverseY())
	//{
	//	DrawString(draw_text_pos_x + text_space_y + 100, draw_text_pos_y + text_space_y * padStickReverseY, "NORMAL", 0xffffff);
	//}
	//else
	//{
	//	DrawString(draw_text_pos_x + text_space_y + 100, draw_text_pos_y + text_space_y * padStickReverseY, "REVERSE", 0xffffff);
	//}
}