#include "ConfigScene.h"
#include "SceneManager.h"
#include "DebugScene.h"
#include "../Util/InputState.h"
#include "../Util/SoundManager.h"
#include "../Util/SaveData.h"
#include "../common.h"
#include <DxLib.h>

namespace
{
	// 表示するテキストの全体の位置
	constexpr int draw_text_pos_x = common::screen_width / 2 - 100;
	constexpr int draw_text_pos_y = common::screen_height / 2 - 100;

	// テキストの文字間
	constexpr int text_space = 64;
}

// コンストラクタ
ConfigScene::ConfigScene(SceneManager& manager) :
	Scene(manager),
	currentSelectItem_(0)
{
	// フェードを行わないようフェードしてない状態で開始
	fadeBright_ = 0;

	// BGMを鳴らす
	SoundManager::GetInstance().PlayBGM("bgmTest");
}

// デストラクタ
ConfigScene::~ConfigScene()
{
	// 処理なし
}

// 更新
void ConfigScene::Update()
{
	// 選択肢を回す処理
	// フェード中の場合は処理を行わない
	int itemTotalValue = static_cast<int>(Item::TOTAL_VALUE);
	if (InputState::IsTriggered(InputType::UP) && !IsFadeing())
	{
		currentSelectItem_ = ((currentSelectItem_ - 1) + itemTotalValue) % itemTotalValue;
	}
	else if (InputState::IsTriggered(InputType::DOWN) && !IsFadeing())
	{
		currentSelectItem_ = (currentSelectItem_ + 1) % itemTotalValue;
	}

	// 選択されているタイプの音量の調節
	if (InputState::IsTriggered(InputType::RIGHT))
	{
		if (currentSelectItem_ == static_cast<int>(Item::WHOLE_VOLUME))
		{
			// 全体音量コンフィグの増加
			// 音量の最大値より大きくなったら最小値にする
			SaveData::GetInstance().SetWholeVolume();
	
			// 設定したコンフィグから音量調節
			SoundManager::GetInstance().SetVolume("bgmTest", 255);
		}
		else if (currentSelectItem_ == static_cast<int>(Item::BGM_VOLUME))
		{
			// BGM音量コンフィグの増加
			// 音量の最大値より大きくなったら最小値にする
			SaveData::GetInstance().SetBgmVolume();

			// 設定したコンフィグから音量調節
			SoundManager::GetInstance().SetVolume("bgmTest", 255);
		}
		else if (currentSelectItem_ == static_cast<int>(Item::SE_VOLUME))
		{
			// SE音量コンフィグの増加
			// 音量の最大値より大きくなったら最小値にする
			SaveData::GetInstance().SetSeVolume();

			// SEは変更するたびにサウンドを鳴らす
			// 設定したコンフィグから音量調節
			SoundManager::GetInstance().Play("cursorTest");
		}
		else if (currentSelectItem_ == static_cast<int>(Item::PAD_STICK_SENS_X))
		{
			// 感度の最大値より大きくなったら最小値にする
			SaveData::GetInstance().SetPadStickSensitivityX();
		}
		else if (currentSelectItem_ == static_cast<int>(Item::PAD_STICK_SENS_Y))
		{
			// 感度の最大値より大きくなったら最小値にする
			SaveData::GetInstance().SetPadStickSensitivityY();
		}
		else if (currentSelectItem_ == static_cast<int>(Item::PAD_STICK_REVERSE_X))
		{
			SaveData::GetInstance().SetPadStickReverseX();
		}
		else if (currentSelectItem_ == static_cast<int>(Item::PAD_STICK_REVERSE_Y))
		{
			SaveData::GetInstance().SetPadStickReverseY();
		}
	}

	// 前の画面に戻る
	if (InputState::IsTriggered(InputType::BACK))
	{
		// シーン遷移するのでサウンドを止める
		SoundManager::GetInstance().StopAllSound();

#ifdef _DEBUG
		manager_.PopScene();
#else 

#endif
		return;
	}
	// フェードの更新
	UpdateFade();
}

// 描画
void ConfigScene::Draw()
{
	DrawBox(0, 0, common::screen_width, common::screen_height, 0xd0d0d0, true);

	// 現在のシーンのテキスト表示
	DrawString(0, 0, "ConfigScene", 0x000000, true);

	// BGMの項目と音量の表示
	int whole = static_cast<int>(Item::WHOLE_VOLUME);
	DrawString(draw_text_pos_x, draw_text_pos_y + text_space * whole, "全体音量", 0x000000, true);
	DrawFormatString(draw_text_pos_x + text_space + 50, draw_text_pos_y + text_space * whole, 0x000000, "%d", SaveData::GetInstance().GetSaveData().wholeVolume);

	// BGMの項目と音量の表示
	int bgm = static_cast<int>(Item::BGM_VOLUME);
	DrawString(draw_text_pos_x, draw_text_pos_y + text_space * bgm, "BGM", 0x000000, true);
	DrawFormatString(draw_text_pos_x + text_space, draw_text_pos_y + text_space * bgm, 0x000000, "%d", SaveData::GetInstance().GetBgmVolume());

	// SEの項目と音量の表示
	int se = static_cast<int>(Item::SE_VOLUME);
	DrawString(draw_text_pos_x, draw_text_pos_y + text_space * se, "SE", 0x000000, true);
	DrawFormatString(draw_text_pos_x + text_space, draw_text_pos_y + text_space * se, 0x000000, "%d", SaveData::GetInstance().GetSeVolume());

	// スティックの感度Xの項目と音量の表示
	int padStickX = static_cast<int>(Item::PAD_STICK_SENS_X);
	DrawString(draw_text_pos_x, draw_text_pos_y + text_space * padStickX, "パッドの横感度", 0x000000, true);
	DrawFormatString(draw_text_pos_x + text_space + 100, draw_text_pos_y + text_space * padStickX, 0x000000, "%d", SaveData::GetInstance().GetPadStickSensitivityX());

	// スティックの感度Yの項目と音量の表示
	int padStickY = static_cast<int>(Item::PAD_STICK_SENS_Y);
	DrawString(draw_text_pos_x, draw_text_pos_y + text_space * padStickY, "パッドの縦感度", 0x000000, true);
	DrawFormatString(draw_text_pos_x + text_space + 100, draw_text_pos_y + text_space * padStickY, 0x000000, "%d", SaveData::GetInstance().GetPadStickSensitivityY());

	int padStickReverseX = static_cast<int>(Item::PAD_STICK_REVERSE_X);
	DrawString(draw_text_pos_x, draw_text_pos_y + text_space * padStickReverseX, "パッドの横リバース", 0x000000, true);
	if (!SaveData::GetInstance().GetPadStickReverseX())
	{
		DrawString(draw_text_pos_x + text_space + 100, draw_text_pos_y + text_space * padStickReverseX, "NORMAL", 0x000000);
	}
	else
	{
		DrawString(draw_text_pos_x + text_space + 100, draw_text_pos_y + text_space * padStickReverseX, "REVERSE", 0x000000);
	}

	int padStickReverseY = static_cast<int>(Item::PAD_STICK_REVERSE_Y);
	DrawString(draw_text_pos_x, draw_text_pos_y + text_space * padStickReverseY, "パッドの縦リバース", 0x000000, true);
	if (!SaveData::GetInstance().GetPadStickReverseY())
	{
		DrawString(draw_text_pos_x + text_space + 100, draw_text_pos_y + text_space * padStickReverseY, "NORMAL", 0x000000);
	}
	else
	{
		DrawString(draw_text_pos_x + text_space + 100, draw_text_pos_y + text_space * padStickReverseY, "REVERSE", 0x000000);
	}

	// 現在選択中の項目の横に→を表示
	DrawString(draw_text_pos_x - text_space, draw_text_pos_y + text_space * currentSelectItem_, "→", 0xff0000);

	// フェードの描画
	DrawFade();
}