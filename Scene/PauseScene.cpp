#include "PauseScene.h"
#include "SceneManager.h"
#include "SoundSettingScene.h"
#include "DebugScene.h"
#include "TitleScene.h"
#include "../Util/InputState.h"
#include "../Game.h"
#include <DxLib.h>

namespace
{
	// ポーズウィンドウ
	constexpr int window_width = Game::screen_width;	//ポーズ枠の幅
	constexpr int window_height = Game::screen_height;	//ポーズ枠の高さ
	constexpr int window_start_x = (Game::screen_width - window_width) / 2;		//ポーズ枠の左
	constexpr int window_start_y = (Game::screen_height - window_height) / 2;	//ポーズ枠上

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
PauseScene::PauseScene(SceneManager& manager) :
	Scene(manager),
	currentSelectItem_(0)
{
	fadeBright_ = 0;
}

/// <summary>
/// デストラクタ
/// </summary>
PauseScene::~PauseScene()
{
}

/// <summary>
/// 更新
/// </summary>
void PauseScene::Update()
{
	// フェードアウトが終わりしだい選択されたシーンに飛ぶ
	if (isFadeOut_ && !IsFadingOut())
	{
		if (currentSelectItem_ == static_cast<int>(Item::DEBUG_SCENE))
		{
			manager_.PopAllSceneAndChangeScene(new DebugScene(manager_));
			return;
		}
	}

	// 選択肢を回す処理
	if (InputState::IsTriggered(InputType::UP) && !isFadeOut_)
	{
		currentSelectItem_ = ((currentSelectItem_ - 1) + static_cast<int>(Item::NUM)) % static_cast<int>(Item::NUM);
	}
	else if (InputState::IsTriggered(InputType::DOWN) && !isFadeOut_)
	{
		currentSelectItem_ = (currentSelectItem_ + 1) % static_cast<int>(Item::NUM);
	}

	// 決定ボタンを押したらフェードアウト開始
	if (InputState::IsTriggered(InputType::DECISION) && !IsFadingIn())
	{
		StartFadeOut();
		isFadeOut_ = true;
	}

	// ポーズ画面から戻る
	if (InputState::IsTriggered(InputType::BACK) || InputState::IsTriggered(InputType::PAUSE))
	{
		manager_.PopScene();
		return;
	}

	// フェードの更新
	UpdateFade();
}

/// <summary>
/// 描画
/// </summary>
void PauseScene::Draw()
{
	//ポーズウィンドウ
	DrawBox(window_start_x, window_start_y, window_start_x + window_width, window_start_y + window_height, 0xd0d0d0, true);
	DrawBox(window_start_x, window_start_y, window_start_x + window_width, window_start_y + window_height, 0xffffff, false);

	//ポーズ中メッセージ
	DrawString(window_start_x + 10, window_start_y + 10, "Pausing...", 0xffff88);

	DrawString(draw_text_pos_x, draw_text_pos_y + text_space * static_cast<int>(Item::DEBUG_SCENE), "DebugScene", 0x000000, true);
	DrawString(draw_text_pos_x - 32, draw_text_pos_y + text_space * currentSelectItem_, "→", 0xff0000);

	// フェードの描画
	DrawFade();
}