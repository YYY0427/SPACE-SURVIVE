#include <DxLib.h>
#include <cassert>
#include "TitleScene.h"
#include "SceneManager.h"
#include "MainScene.h"
#include "OptionScene.h"
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
	constexpr int text_space_y = 70;
}

// コンストラクタ
TitleScene::TitleScene(SceneManager& manager) :
	Scene(manager),
	updateFunc_(&TitleScene::NormalUpdate),
	currentSelectItem_(0),
	isDoFade_(true)
{
	handle_ = LoadGraph("Data/title.png");

	// BGMを鳴らす
	SoundManager::GetInstance().PlayBGM("bgmTest");
}

// デストラクタ
TitleScene::~TitleScene()
{
	// 処理なし
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
	int sceneItemTotalValue = static_cast<int>(Item::TOTAL_VALUE);
	if (InputState::IsTriggered(InputType::UP))
	{
		currentSelectItem_ = ((currentSelectItem_ - 1) + sceneItemTotalValue) % sceneItemTotalValue;
	}
	else if (InputState::IsTriggered(InputType::DOWN) && !IsFadeing())
	{
		currentSelectItem_ = (currentSelectItem_ + 1) % sceneItemTotalValue;
	}

	// 次へのボタンが押されてたらフェードアウトの開始
	if (InputState::IsTriggered(InputType::DECISION))
	{
		// フェードアウトの開始
		StartFadeOut(255);
	}
	// フェードアウトが終わり次第シーン遷移
	if (IsStartFadeOutAfterFadingOut())
	{
		switch (currentSelectItem_)
		{
		case static_cast<int>(Item::START):
			manager_.ChangeScene(new MainScene(manager_));
			return;
		case static_cast<int>(Item::OPSITON):
			manager_.PushScene(new OptionScene(manager_));
			break;
		case static_cast<int>(Item::END):
			manager_.SetIsGameEnd(true);
			return;
		default:
			assert(0);
		}
		// PushSceneした場合シーンが残ったままなので
		// フェードインを開始する
		StartFadeIn();
	}
	// オプションシーンに遷移する場合はフェードアウトを通常の半分の状態で止めて遷移する
	// オプションシーンの背景をモザイク状態で残すため
	/*const bool isOption = currentSelectItem_ == static_cast<int>(Item::OPSITON);
	const bool isHalfFade = fadeBright_ > 255 / 2;
	const bool isOpsionFade = isOption && isHalfFade && isFadeOut_;
	if (isOpsionFade)
	{
		fadeSpeed_ = 0;
	}*/
	// フェードの更新
	UpdateFade();
}

// 描画
void TitleScene::Draw()
{
	DrawGraph(0, 0, handle_, true);

	// 現在のシーンのテキスト描画
	DrawString(0, 0, "TitleScene", 0xffffff, true);

	// 項目の描画
	auto& stringManager = StringManager::GetInstance();
	stringManager.DrawStringCenter("TitleItemStart", common::screen_width / 2, draw_text_pos_y + text_space_y * static_cast<int>(Item::START), 0xffffff);
	stringManager.DrawStringCenter("TitleItemOption", common::screen_width / 2, draw_text_pos_y + text_space_y * static_cast<int>(Item::OPSITON), 0xffffff);
	stringManager.DrawStringCenter("TitleItemEnd", common::screen_width / 2, draw_text_pos_y + text_space_y * static_cast<int>(Item::END), 0xffffff);

	// 選択中の項目にバーを描画
	stringManager.DrawString("TitleItemSelectBarRight", common::screen_width / 2 - 100, draw_text_pos_y + text_space_y * currentSelectItem_, 0xffffff);
	stringManager.DrawString("TitleItemSelectBarLeft", common::screen_width / 2  + 90, draw_text_pos_y + text_space_y * currentSelectItem_, 0xffffff);

	// フェードの描画
	DrawGaussFade(true);

	// フェードの描画
	DrawFade(true);
}