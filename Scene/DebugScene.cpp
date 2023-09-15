#include "DebugScene.h"
#include "SceneManager.h"
#include "TitleScene.h"
#include "MainScene.h"
#include "PauseScene.h"
#include "SoundSettingScene.h"
#include "TestScene.h"
#include "../Util/InputState.h"
#include "../Game.h"

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
/// <param name="manager">シーンマネーシャーへの参照</param>
DebugScene::DebugScene(SceneManager& manager):
	Scene(manager),
	currentSelectIndex_(0)
{
}

/// <summary>
/// デストラクタ
/// </summary>
DebugScene::~DebugScene()
{
}

/// <summary>
/// 更新
/// </summary>
void DebugScene::Update()
{
	// フェードアウトが終わりしだい選択されたシーンに飛ぶ
	if (isFadeOut_ && !IsFadingOut())
	{
		if (currentSelectIndex_ == TEST_SCENE)
		{
			manager_.ChangeScene(new TestScene(manager_));
			return;
		}
		else if (currentSelectIndex_ == TITLE_SCENE)
		{
			manager_.ChangeScene(new TitleScene(manager_));
			return;
		}
		else if (currentSelectIndex_ == MAIN_SCENE)
		{
			manager_.ChangeScene(new MainScene(manager_));
			return;
		}
		else if (currentSelectIndex_ == SOUNDSETTING_SCENE)
		{
			manager_.ChangeScene(new SoundSettingScene(manager_));
			return;
		}
		else if (currentSelectIndex_ == PAUSE_SCENE)
		{
			// ポーズの場合シーンが残っているので初期化
			isFadeOut_ = false;

			manager_.PushScene(new PauseScene(manager_));
			return;
		}
	}

	// 選択肢を回す処理
	if (InputState::IsTriggered(InputType::UP) && !isFadeOut_)
	{
		currentSelectIndex_ = ((currentSelectIndex_ - 1) + NUM) % NUM;
	}
	else if (InputState::IsTriggered(InputType::DOWN) && !isFadeOut_)
	{
		currentSelectIndex_ = (currentSelectIndex_ + 1) % NUM;
	}

	// 決定ボタンを押したらフェードアウト開始
	if (InputState::IsTriggered(InputType::DECISION) && !IsFadingIn())
	{
		// ポーズの場合はフェードを行わない
		if (currentSelectIndex_ != PAUSE_SCENE)
		{
			StartFadeOut();
		}
		isFadeOut_ = true;
	}

	// フェードの更新
	UpdateFade();
}

/// <summary>
///  描画
/// </summary>
void DebugScene::Draw()
{
	// テキストの表示
	DrawString(0, 0, "DebugScene", 0xffffff, true);
	DrawString(draw_text_pos_x, draw_text_pos_y + text_space * TEST_SCENE, "TestScene", 0xffffff, true);
	DrawString(draw_text_pos_x, draw_text_pos_y + text_space * TITLE_SCENE, "TitleScene", 0xffffff, true);
	DrawString(draw_text_pos_x, draw_text_pos_y + text_space * MAIN_SCENE, "MainScene", 0xffffff, true);
	DrawString(draw_text_pos_x, draw_text_pos_y + text_space * SOUNDSETTING_SCENE, "SoundSettingScene", 0xffffff, true);
	DrawString(draw_text_pos_x, draw_text_pos_y + text_space * PAUSE_SCENE, "PauseScene", 0xffffff, true);

	DrawString(draw_text_pos_x - 32, draw_text_pos_y + text_space * currentSelectIndex_, "→", 0xff0000);

	// フェードの描画
	DrawFade();
}