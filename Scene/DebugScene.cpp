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
	currentSelectItem_(0)
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
		if (currentSelectItem_ == static_cast<int>(SceneItem::TEST_SCENE))
		{
			manager_.ChangeScene(new TestScene(manager_));
			return;
		}
		else if (currentSelectItem_ == static_cast<int>(SceneItem::TITLE_SCENE))
		{
			manager_.ChangeScene(new TitleScene(manager_));
			return;
		}
		else if (currentSelectItem_ == static_cast<int>(SceneItem::MAIN_SCENE))
		{
			manager_.ChangeScene(new MainScene(manager_));
			return;
		}
		else if (currentSelectItem_ == static_cast<int>(SceneItem::SOUNDSETTING_SCENE))
		{
			manager_.ChangeScene(new SoundSettingScene(manager_));
			return;
		}
		else if (currentSelectItem_ == static_cast<int>(SceneItem::PAUSE_SCENE))
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
		currentSelectItem_ = ((currentSelectItem_ - 1) + static_cast<int>(SceneItem::NUM)) % static_cast<int>(SceneItem::NUM);
	}
	else if (InputState::IsTriggered(InputType::DOWN) && !isFadeOut_)
	{
		currentSelectItem_ = (currentSelectItem_ + 1) % static_cast<int>(SceneItem::NUM);
	}

	// 決定ボタンを押したらフェードアウト開始
	if (InputState::IsTriggered(InputType::DECISION) && !IsFadingIn())
	{
		// ポーズの場合はフェードを行わない
		if (currentSelectItem_ != static_cast<int>(SceneItem::PAUSE_SCENE))
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
	DrawString(draw_text_pos_x, draw_text_pos_y + text_space * static_cast<int>(SceneItem::TEST_SCENE), "TestScene", 0xffffff, true);
	DrawString(draw_text_pos_x, draw_text_pos_y + text_space * static_cast<int>(SceneItem::TITLE_SCENE), "TitleScene", 0xffffff, true);
	DrawString(draw_text_pos_x, draw_text_pos_y + text_space * static_cast<int>(SceneItem::MAIN_SCENE), "MainScene", 0xffffff, true);
	DrawString(draw_text_pos_x, draw_text_pos_y + text_space * static_cast<int>(SceneItem::SOUNDSETTING_SCENE), "SoundSettingScene", 0xffffff, true);
	DrawString(draw_text_pos_x, draw_text_pos_y + text_space * static_cast<int>(SceneItem::PAUSE_SCENE), "PauseScene", 0xffffff, true);

	DrawString(draw_text_pos_x - 32, draw_text_pos_y + text_space * currentSelectItem_, "→", 0xff0000);

	// フェードの描画
	DrawFade();
}