#include "PadSettingScene.h"
#include "SceneManager.h"
#include "../Util/StringManager.h"
#include "../Util/InputState.h"
#include "../Util/SaveData.h"
#include "../common.h"
#include <cassert>

namespace
{
	// 表示するテキストの全体の位置
	constexpr int draw_text_pos_x = 350;
	constexpr int draw_text_pos_y = common::screen_height / 2 - 120;

	// テキストの文字間
	constexpr int text_space_y = 64;

	// 選択されていないときの描画色
	constexpr int normal_color = 0x666666;

	// 選択されているときの描画色
	constexpr int choose_color = 0xffffff;
}

PadSettingScene::PadSettingScene(SceneManager& manager) : 
	Scene(manager),
	currentSelectItem_(0),
	itemColorTable_({})
{
	for (auto& item : itemColorTable_)
	{
		item = normal_color;
	}
}

PadSettingScene::~PadSettingScene()
{
}

void PadSettingScene::Update()
{
	for (auto& item : itemColorTable_)
	{
		item = normal_color;
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

	itemColorTable_[currentSelectItem_] = choose_color;

	switch (currentSelectItem_)
	{
	case static_cast<int>(Item::PAD_SENS_X):
		SaveData::GetInstance().SetPadStickSensitivityX();
		break;
	case static_cast<int>(Item::PAD_SENS_Y):
		SaveData::GetInstance().SetPadStickSensitivityY();
		break;
	case static_cast<int>(Item::PAD_REVERSE_X):
		SaveData::GetInstance().SetPadStickReverseX();
		break;
	case static_cast<int>(Item::PAD_REVERSE_Y):
		SaveData::GetInstance().SetPadStickReverseY();
		break;
	case static_cast<int>(Item::BACK):
		if(InputState::IsTriggered(InputType::DECISION)) manager_.PopScene();
		return;
	default:
		assert(0);
	}

	if (InputState::IsTriggered(InputType::BACK))
	{
		manager_.PopScene();
		return;
	}
}

void PadSettingScene::Draw()
{
	// インスタンスの取得
	auto& stringManager = StringManager::GetInstance();
	auto& saveData = SaveData::GetInstance();

	// タイトルの描画
	stringManager.DrawStringCenter("PadSettingTitle", common::screen_width / 2, 100, 0xffffff);

	// 項目の描画
	int padSensXItemColor = static_cast<int>(Item::PAD_SENS_X);
	int padSensYItemColor = static_cast<int>(Item::PAD_SENS_Y);
	int padReverseXItemColor = static_cast<int>(Item::PAD_REVERSE_X);
	int padReverseYItemColor = static_cast<int>(Item::PAD_REVERSE_Y);
	int backItemColor = static_cast<int>(Item::BACK);
	stringManager.DrawStringCenter("PadSettingItemSensX", draw_text_pos_x, draw_text_pos_y + text_space_y * padSensXItemColor, itemColorTable_[padSensXItemColor]);
	stringManager.DrawStringCenter("PadSettingItemSensY", draw_text_pos_x, draw_text_pos_y + text_space_y * padSensYItemColor, itemColorTable_[padSensYItemColor]);
	stringManager.DrawStringCenter("PadSettingItemReverseX", draw_text_pos_x, draw_text_pos_y + text_space_y * padReverseXItemColor, itemColorTable_[padReverseXItemColor]);
	stringManager.DrawStringCenter("PadSettingItemReverseY", draw_text_pos_x, draw_text_pos_y + text_space_y * padReverseYItemColor, itemColorTable_[padReverseYItemColor]);
	stringManager.DrawStringCenter("PadSettingItemBack", common::screen_width / 2, draw_text_pos_y + text_space_y * backItemColor, itemColorTable_[backItemColor]);
	
	stringManager.DrawFormatStringCenter("PadSettingNumber", saveData.GetSaveData().padStickSensitivityX, common::screen_width / 2, draw_text_pos_y + text_space_y * padSensXItemColor, itemColorTable_[padSensXItemColor]);
	stringManager.DrawFormatStringCenter("PadSettingNumber", saveData.GetSaveData().padStickSensitivityY, common::screen_width / 2, draw_text_pos_y + text_space_y * padSensYItemColor, itemColorTable_[padSensYItemColor]);

	DrawSwitch(saveData.GetSaveData().padStickReverseX, padReverseXItemColor);
	DrawSwitch(saveData.GetSaveData().padStickReverseY, padReverseYItemColor);
}

void PadSettingScene::DrawSwitch(bool isOn, int item)
{
	auto& stringManager = StringManager::GetInstance();
	if (isOn)
	{
		stringManager.DrawStringCenter("PadSettingItemReverseOn", common::screen_width / 2, draw_text_pos_y + text_space_y * item, itemColorTable_[item]);
	}
	else
	{
		stringManager.DrawStringCenter("PadSettingItemReverseOff", common::screen_width / 2, draw_text_pos_y + text_space_y * item, itemColorTable_[item]);
	}
}
