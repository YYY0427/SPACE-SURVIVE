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
	constexpr int text_space = 32;
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
		if (currentSelectItem_ == static_cast<int>(Item::BGM))
		{
			// BGM音量コンフィグの増加
			// 音量の最大値より大きくなったら最小値にする
			SaveData::GetInstance().SetBgmVolume();

			// 設定したコンフィグから音量調節
			SoundManager::GetInstance().SetVolume("bgmTest", 255);
		}
		else if (currentSelectItem_ == static_cast<int>(Item::SE))
		{
			// SE音量コンフィグの増加
			// 音量の最大値より大きくなったら最小値にする
			SaveData::GetInstance().SetSeVolume();

			// SEは変更するたびにサウンドを鳴らす
			// 設定したコンフィグから音量調節
			SoundManager::GetInstance().Play("cursorTest");
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
	int bgm = static_cast<int>(Item::BGM);
	DrawString(draw_text_pos_x, draw_text_pos_y + text_space * bgm, "BGM", 0x000000, true);
	DrawFormatString(draw_text_pos_x + 32, draw_text_pos_y + text_space * bgm, 0x000000, "%d", SaveData::GetInstance().GetBgmVolume());

	// SEの項目と音量の表示
	int se = static_cast<int>(Item::SE);
	DrawString(draw_text_pos_x, draw_text_pos_y + text_space * se, "SE", 0x000000, true);
	DrawFormatString(draw_text_pos_x + 32, draw_text_pos_y + text_space * se, 0x000000, "%d", SaveData::GetInstance().GetSeVolume());

	// 現在選択中の項目の横に→を表示
	DrawString(draw_text_pos_x - 32, draw_text_pos_y + text_space * currentSelectItem_, "→", 0xff0000);

	// フェードの描画
	DrawFade();
}