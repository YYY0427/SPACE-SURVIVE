#include "PauseScene.h"
#include "SceneManager.h"
#include "ConfigScene.h"
#include "DebugScene.h"
#include "TitleScene.h"
#include "../Util/InputState.h"
#include "../common.h"
#include <DxLib.h>

namespace
{
	// ポーズウィンドウ枠の幅
	constexpr int window_width = common::screen_width;

	// ポーズウィンドウ枠の高さ
	constexpr int window_height = common::screen_height;	

	// ポーズウィンドウの左上の位置(ウィンドウの開始位置)
	constexpr int window_start_x = 0;		
	constexpr int window_start_y = 0;	

	// 表示するテキストの全体の位置
	constexpr int draw_text_pos_x = common::screen_width / 2 - 100;
	constexpr int draw_text_pos_y = common::screen_height / 2 - 100;

	// テキストの文字間
	constexpr int text_space = 32;
}

// コンストラクタ
PauseScene::PauseScene(SceneManager& manager) :
	Scene(manager),
	currentSelectItem_(0)
{
	// フェードを行わないようフェードしてない状態で開始
	fadeBright_ = 0;
}

// デストラクタ
PauseScene::~PauseScene()
{
	// 処理なし
}

// 更新
void PauseScene::Update()
{
	

	// 選択肢を回す処理
	// フェード中の場合は処理を行わない
	if (InputState::IsTriggered(InputType::UP) && !IsFadeing())
	{
		currentSelectItem_ = ((currentSelectItem_ - 1) + static_cast<int>(Item::TOTAL_VALUE)) % static_cast<int>(Item::TOTAL_VALUE);
	}
	else if (InputState::IsTriggered(InputType::DOWN) && !IsFadeing())
	{
		currentSelectItem_ = (currentSelectItem_ + 1) % static_cast<int>(Item::TOTAL_VALUE);
	}
	// 決定ボタンを押したらフェードアウト開始
	if (InputState::IsTriggered(InputType::DECISION) && !IsFadeing())
	{
		// フェードアウト開始
		StartFadeOut();

		// フェードアウトが行われたかどうかのフラグを立てる
		// シーン遷移の際、フェードアウトが行われたかどうかを確認するため
		isFadeOut_ = true;
	}
	// フェードアウトが終わりしだい選択された項目に飛ぶ
	if (isFadeOut_ && !IsFadingOut())
	{
		if (currentSelectItem_ == static_cast<int>(Item::DEBUG_SCENE))
		{
			manager_.PopAllSceneAndChangeScene(new DebugScene(manager_));
			return;
		}
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

// 描画
void PauseScene::Draw()
{
	// ポーズウィンドウ
	DrawBox(window_start_x, window_start_y, window_start_x + window_width, window_start_y + window_height, 0xd0d0d0, true);
	DrawBox(window_start_x, window_start_y, window_start_x + window_width, window_start_y + window_height, 0xffffff, false);

	// 現在のシーンのテキスト表示
	DrawString(window_start_x + 10, window_start_y + 10, "PauseScene", 0xffff88);

	// デバッグシーンから飛べる項目のテキスト表示
	DrawString(draw_text_pos_x, draw_text_pos_y + text_space * static_cast<int>(Item::DEBUG_SCENE), "DebugScene", 0x000000, true);

	// 現在選択中の項目の横に→を表示
	DrawString(draw_text_pos_x - 32, draw_text_pos_y + text_space * currentSelectItem_, "→", 0xff0000);

	// フェードの描画
	DrawFade();
}