#include "PauseScene.h"
#include "SceneManager.h"
#include "OptionScene.h"
#include "TitleScene.h"
#include "../Util/InputState.h"
#include "../Util/SoundManager.h"
#include "../Util/StringManager.h"
#include "../common.h"
#include <DxLib.h>

namespace
{
	// 表示するテキストの全体の位置
	constexpr int draw_text_pos_x = common::screen_width / 2;
	constexpr int draw_text_pos_y = common::screen_height / 2 - 100;

	// テキストの文字間
	constexpr int text_space_y = 64;
}

// コンストラクタ
PauseScene::PauseScene(SceneManager& manager) :
	Scene(manager),
	currentSelectItem_(0)
{
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
	if (InputState::IsTriggered(InputType::UP))
	{
		currentSelectItem_ = ((currentSelectItem_ - 1) + static_cast<int>(Item::TOTAL_VALUE)) % static_cast<int>(Item::TOTAL_VALUE);
	}
	else if (InputState::IsTriggered(InputType::DOWN))
	{
		currentSelectItem_ = (currentSelectItem_ + 1) % static_cast<int>(Item::TOTAL_VALUE);
	}
	// 決定ボタンを押したらフェードアウト開始
	if (InputState::IsTriggered(InputType::DECISION))
	{
		// フェードアウト開始
		StartFadeOut();
	}
	// フェードアウトが終わりしだい選択された項目に飛ぶ
	if (IsStartFadeOutAfterFadingOut())
	{
		switch (currentSelectItem_)
		{
		case static_cast<int>(Item::CONTINUE):
			manager_.PopScene();
			return;
		case static_cast<int>(Item::OPTION):
			manager_.PushScene(new OptionScene(manager_));
			break;
		case static_cast<int>(Item::TITLE):
			// タイトルに戻るので全てのサウンドを止める
			SoundManager::GetInstance().StopAllSound();
			manager_.ChangeScene(new TitleScene(manager_));
			return;
		}
		// PushSceneした場合シーンが残ったままなので
		// フェードインを開始する
		StartFadeIn();
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
	auto& stringManager = StringManager::GetInstance();
	stringManager.DrawStringCenter("PauseTitle", draw_text_pos_x, 100, 0xffffff);
	stringManager.DrawStringCenter("PauseItemContinue", draw_text_pos_x, draw_text_pos_y + text_space_y * static_cast<int>(Item::CONTINUE), 0xffffff);
	stringManager.DrawStringCenter("PauseItemOption", draw_text_pos_x, draw_text_pos_y + text_space_y * static_cast<int>(Item::OPTION), 0xffffff);
	stringManager.DrawStringCenter("PauseItemTitle", draw_text_pos_x, draw_text_pos_y + text_space_y * static_cast<int>(Item::TITLE), 0xffffff);
	stringManager.DrawStringCenter("PausetemSelectBarRight", draw_text_pos_x - 100, draw_text_pos_y + text_space_y * currentSelectItem_, 0xffffff);
	stringManager.DrawStringCenter("PausetemSelectBarLeft", draw_text_pos_x + 100, draw_text_pos_y + text_space_y * currentSelectItem_, 0xffffff);

	// フェードの描画
	DrawFade();

	// モザイクフェードの描画
	DrawGaussFade();
}