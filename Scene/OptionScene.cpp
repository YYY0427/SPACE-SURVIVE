#include "OptionScene.h"
#include "SceneManager.h"
#include "DebugScene.h"
#include "PadSettingScene.h"
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
	constexpr int normal_color = 0x666666;

	// 選択されているときの描画色
	constexpr int choose_color = 0xffffff;
}

// コンストラクタ
OptionScene::OptionScene(SceneManager& manager) :
	Scene(manager),
	currentSelectItem_(0),
	soundIconImgHandle_(-1)
{
	SetFadeBright(0);

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
		SaveData::GetInstance().SetWindowMode();
		break;
	case static_cast<int>(Item::MASTER_VOLUME):
		// 全体音量の調整
		SaveData::GetInstance().SetMasterVolume();
		break;
	case static_cast<int>(Item::BGM_VOLUME):
		// BGM音量の調整
		SaveData::GetInstance().SetBgmVolume();
		break;
	case static_cast<int>(Item::SE_VOLUME):
		// SE音量の調整
		SaveData::GetInstance().SetSeVolume();
		break;
	case static_cast<int>(Item::PAD_SETTING):
		if(InputState::IsTriggered(InputType::DECISION)) StartFadeOut(255, 16);
		break;
	case static_cast<int>(Item::BACK):
		if (InputState::IsTriggered(InputType::DECISION)) manager_.PopScene();
		return;
	}
	
	if (IsStartFadeOutAfterFadingOut())
	{
		StartFadeIn();
		manager_.PushScene(new PadSettingScene(manager_));
		return;
	}

	if (InputState::IsTriggered(InputType::BACK))
	{
		manager_.PopScene();
		return;
	}

	// フェードの更新
	UpdateFade();
}

// 描画
void OptionScene::Draw()
{
	// インスタンスの取得
	auto& stringManager = StringManager::GetInstance();
	auto& saveData = SaveData::GetInstance();

	// 描画透明度の設定
	int fade = 255 - GetFadeBright();
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, fade);

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

	// ウィンドウモードの状態の表示
	if (saveData.GetSaveData().windowMode)
	{
		stringManager.DrawStringCenter("OptionItemWindowModeOff", common::screen_width / 2, draw_text_pos_y + text_space_y * windowMode, itemColorDataTable_[windowMode]);
	}
	else
	{
		stringManager.DrawStringCenter("OptionItemWindowModeOn", common::screen_width / 2, draw_text_pos_y + text_space_y * windowMode, itemColorDataTable_[windowMode]);
	}

	// 各サウンド音量の表示
	std::array<int, 3> volume = {};
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

	// 描画の設定の初期化
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}