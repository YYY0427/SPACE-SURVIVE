#include <DxLib.h>
#include "TitleScene.h"
#include "SceneManager.h"
#include "MainScene.h"
#include "ConfigScene.h"
#include "DebugScene.h"
#include "../Util/InputState.h"
#include "../Util/SoundManager.h"
#include "../Util/DrawFunctions.h"
#include "../Util/StringManager.h"
#include "../common.h"

namespace
{
	// 表示するテキストの全体の位置
	constexpr int draw_text_pos_y = common::screen_height / 2 + 100;

	// テキストの文字間
	constexpr int text_space = 70;
}

// コンストラクタ
TitleScene::TitleScene(SceneManager& manager) :
	Scene(manager),
	updateFunc_(&TitleScene::NormalUpdate),
	currentSelectItem_(0)
{
}

// デストラクタ
TitleScene::~TitleScene()
{
}

// メンバ関数ポインタの更新
void TitleScene::Update()
{
	(this->*updateFunc_)();
}

// 通常の更新
void TitleScene::NormalUpdate()
{


	// 選択肢を回す処理
	// フェード中の場合は処理を行わない
	int sceneItemTotalValue = static_cast<int>(Item::TOTAL_VALUE);
	if (InputState::IsTriggered(InputType::UP) && !IsFadeing())
	{
		currentSelectItem_ = ((currentSelectItem_ - 1) + sceneItemTotalValue) % sceneItemTotalValue;
	}
	else if (InputState::IsTriggered(InputType::DOWN) && !IsFadeing())
	{
		currentSelectItem_ = (currentSelectItem_ + 1) % sceneItemTotalValue;
	}

	// 次へのボタンが押されて、フェード中でなかったらフェードアウトの開始
	if (InputState::IsTriggered(InputType::DECISION) && !IsFadeing())
	{
		// フェードアウトの開始
		StartFadeOut();

		// フェードアウトが行われたかどうかのフラグを立てる
		// シーン遷移の際、フェードアウトが行われたかどうかを確認するため
		isFadeOut_ = true;
	}
	// フェードアウトが終わり次第シーン遷移
	if (isFadeOut_ && !IsFadingOut())
	{
		if (currentSelectItem_ == static_cast<int>(Item::START))
		{
			isFadeOut_ = false;
			manager_.ChangeScene(new MainScene(manager_));
			return;
		}
		else if (currentSelectItem_ == static_cast<int>(Item::OPSITON))
		{
			isFadeOut_ = false;
			fadeSpeed_ = -8;
			manager_.PushScene(new ConfigScene(manager_));
			return;
		}
		else if (currentSelectItem_ == static_cast<int>(Item::END))
		{
			manager_.SetIsGameEnd(true);
			return;
		}
	}
	// フェードの更新
	UpdateFade();
}

// 描画
void TitleScene::Draw()
{
	// 現在のシーンのテキスト表示
	DrawString(0, 0, "TitleScene", 0xffffff, true);

	auto& stringManager = StringManager::GetInstance();
	stringManager.DrawStringCenter("TitleItemStart", draw_text_pos_y + text_space * static_cast<int>(Item::START), 0xffffff);
	stringManager.DrawStringCenter("TitleItemOption", draw_text_pos_y + text_space * static_cast<int>(Item::OPSITON), 0xffffff);
	stringManager.DrawStringCenter("TitleItemEnd", draw_text_pos_y + text_space * static_cast<int>(Item::END), 0xffffff);

	// 現在選択中の項目の横に→を表示
	DrawString(200, draw_text_pos_y + text_space * currentSelectItem_, "→", 0xff0000);

	// フェードの描画
	DrawGaussFade();

	// フェードの描画
	DrawFade();
}