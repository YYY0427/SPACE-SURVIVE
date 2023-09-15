#include "SoundSettingScene.h"
#include "SceneManager.h"
#include "DebugScene.h"
#include "../Util/InputState.h"
#include "../Util/SoundManager.h"
#include "../Util/SaveData.h"
#include "../Game.h"
#include <DxLib.h>

namespace
{
	// 表示するテキストの全体の位置
	constexpr int draw_text_pos_x = Game::screen_width / 2 - 100;
	constexpr int draw_text_pos_y = Game::screen_height / 2 - 100;

	// テキストの文字間
	constexpr int text_space = 32;
}

/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="manager">シーンマネージャーの参照</param>
SoundSettingScene::SoundSettingScene(SceneManager& manager) :
	Scene(manager),
	currentSelectIndex_(0)
{
	SoundManager::GetInstance().PlayBGM("bgmTest");
}

/// <summary>
/// デストラクタ
/// </summary>
SoundSettingScene::~SoundSettingScene()
{
}

/// <summary>
/// 更新
/// </summary>
void SoundSettingScene::Update(const InputState& input)
{
	//選択肢を回す処理
	if (input.IsTriggered(InputType::UP))
	{
		currentSelectIndex_ = ((currentSelectIndex_ - 1) + NUM) % NUM;
	}
	else if (input.IsTriggered(InputType::DOWN))
	{
		currentSelectIndex_ = (currentSelectIndex_ + 1) % NUM;
	}

	// 選択されているタイプの音量の調節
	if (input.IsTriggered(InputType::RIGHT))
	{
		if (currentSelectIndex_ == BGM)
		{
			SaveData::GetInstance().SetBgmVolume();
			SoundManager::GetInstance().SetVolume("bgmTest", 255);
		}
		else if (currentSelectIndex_ == SE)
		{
			SaveData::GetInstance().SetSeVolume();
			SoundManager::GetInstance().Play("cursorTest");
		}
	}

	// 前の画面に戻る
	if (input.IsTriggered(InputType::BACK))
	{
		// シーン遷移するのでサウンドを止める
		SoundManager::GetInstance().StopAllSound();

#ifdef _DEBUG
		manager_.ChangeScene(new DebugScene(manager_));
#else 
#endif
		return;
	}
}

void SoundSettingScene::Draw()
{
	DrawString(0, 0, "SoundSettingScene", 0xffffff, true);

	DrawString(draw_text_pos_x, draw_text_pos_y + text_space * BGM, "BGM", 0xffffff, true);
	DrawFormatString(draw_text_pos_x + 32, draw_text_pos_y + text_space * BGM, 0xffffff, "%d", SaveData::GetInstance().GetBgmVolume());

	DrawString(draw_text_pos_x, draw_text_pos_y + text_space * SE, "SE", 0xffffff, true);
	DrawFormatString(draw_text_pos_x + 32, draw_text_pos_y + text_space * SE, 0xffffff, "%d", SaveData::GetInstance().GetSeVolume());

	DrawString(draw_text_pos_x - 32, draw_text_pos_y + text_space * currentSelectIndex_, "→", 0xff0000);
}