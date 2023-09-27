#include "DebugScene.h"
#include "SceneManager.h"
#include "TitleScene.h"
#include "MainScene.h"
#include "PauseScene.h"
#include "OptionScene.h"
#include "TestScene.h"
#include "../Util/InputState.h"
#include "../Util/Range.h"
#include "../common.h"

namespace
{
	// 表示するテキストの全体の位置
	constexpr int draw_text_pos_x = common::screen_width / 2 - 100;
	constexpr int draw_text_pos_y = common::screen_height / 2 - 100;

	// テキストの文字間
	constexpr int text_space_y = 32;
}

// コンストラクタ
DebugScene::DebugScene(SceneManager& manager):
	Scene(manager),
	currentSelectItem_(0)
{
	/*scene_[static_cast<int>(SceneItem::TEST_SCENE)] = new TestScene(manager_);
	scene_[static_cast<int>(SceneItem::TITLE_SCENE)] = new TitleScene(manager_);
	scene_[static_cast<int>(SceneItem::MAIN_SCENE)] = new MainScene(manager_);
	scene_[static_cast<int>(SceneItem::OPTION_SCENE)] = new OptionScene(manager_);
	scene_[static_cast<int>(SceneItem::PAUSE_SCENE)] = new PauseScene(manager_);*/
}

// デストラクタ
DebugScene::~DebugScene()
{
	// 処理なし
}

// 更新
void DebugScene::Update()
{

	// 選択肢を回す処理
	int sceneItemTotalValue = static_cast<int>(SceneItem::TOTAL_VALUE);
	if (InputState::IsTriggered(InputType::UP))
	{
		currentSelectItem_ = ((currentSelectItem_ - 1) + sceneItemTotalValue) % sceneItemTotalValue;
	}
	else if (InputState::IsTriggered(InputType::DOWN))
	{
		currentSelectItem_ = (currentSelectItem_ + 1) % sceneItemTotalValue;
	}

	// 決定ボタンを押されて、フェード中ではなかったらフェードアウト開始
	if (InputState::IsTriggered(InputType::DECISION))
	{
		// フェードアウト開始
		StartFadeOut();
	}
	// フェードアウトが終わりしだい選択されたシーンに飛ぶ
	if (IsStartFadeOutAfterFadingOut())
	{
		/*manager_.ChangeScene(scene_[currentSelectItem_]);
		return;*/
		switch (currentSelectItem_)
		{
		case static_cast<int>(SceneItem::TEST_SCENE):
			manager_.ChangeScene(new TestScene(manager_));
			return;
		case static_cast<int>(SceneItem::TITLE_SCENE):
			manager_.ChangeScene(new TitleScene(manager_));
			return;
		case static_cast<int>(SceneItem::MAIN_SCENE):
			manager_.ChangeScene(new MainScene(manager_));
			return;
		case static_cast<int>(SceneItem::OPTION_SCENE):
			// ポーズの場合シーンが残っているので初期化
			isFadeOut_ = false;
			manager_.PushScene(new OptionScene(manager_));
			return;
		case static_cast<int>(SceneItem::PAUSE_SCENE):
			// ポーズの場合シーンが残っているので初期化
			isFadeOut_ = false;
			manager_.PushScene(new PauseScene(manager_));
			return;
		default:
			assert(0);
		}
	}
	// フェードの更新
	UpdateFade();
}

//  描画
void DebugScene::Draw()
{
	// 現在のシーンのテキスト表示
	DrawString(0, 0, "DebugScene", 0xffffff, true);

	// デバッグシーンから飛べるシーンの項目のテキスト表示
	DrawString(draw_text_pos_x, draw_text_pos_y + text_space_y * static_cast<int>(SceneItem::TEST_SCENE), "TestScene", 0xffffff, true);
	DrawString(draw_text_pos_x, draw_text_pos_y + text_space_y * static_cast<int>(SceneItem::TITLE_SCENE), "TitleScene", 0xffffff, true);
	DrawString(draw_text_pos_x, draw_text_pos_y + text_space_y * static_cast<int>(SceneItem::MAIN_SCENE), "MainScene", 0xffffff, true);
	DrawString(draw_text_pos_x, draw_text_pos_y + text_space_y * static_cast<int>(SceneItem::OPTION_SCENE), "ConfigScene", 0xffffff, true);
	DrawString(draw_text_pos_x, draw_text_pos_y + text_space_y * static_cast<int>(SceneItem::PAUSE_SCENE), "PauseScene", 0xffffff, true);

	// 現在選択中の項目の横に→を表示
	DrawString(draw_text_pos_x - text_space_y, draw_text_pos_y + text_space_y * currentSelectItem_, "→", 0xff0000);

	// フェードの描画
	DrawFade();
}