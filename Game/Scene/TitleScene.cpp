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
	SceneBase(manager),
	updateFunc_(&TitleScene::NormalUpdate),
	currentSelectItem_(0)
{
	// 画像のロード
	handle_ = my::MyLoadGraph("Data/Image/title.png");

	// フェードの設定
	fadeDataTable_[static_cast<int>(Item::START)] = { 255, 8, true, true };
	fadeDataTable_[static_cast<int>(Item::END)] = { 255, 8, true, true };
	fadeDataTable_[static_cast<int>(Item::OPSITON)] = { 200, 8, true, true };

	// BGMを鳴らす
	SoundManager::GetInstance().PlayBGM("bgmTest");
}

// デストラクタ
TitleScene::~TitleScene()
{
	DeleteGraph(handle_);
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
	else if (InputState::IsTriggered(InputType::DOWN))
	{
		currentSelectItem_ = (currentSelectItem_ + 1) % sceneItemTotalValue;
	}

	// 次へのボタンが押されてたらフェードアウトの開始
	if (InputState::IsTriggered(InputType::DECISION))
	{
		// フェードアウトの開始
		StartFadeOut(fadeDataTable_[currentSelectItem_].fadeValue, fadeDataTable_[currentSelectItem_].fadeSpeed);
	}

	// フェードアウトが終わり次第シーン遷移
	if (IsStartFadeOutAfterFadingOut())
	{
		switch (static_cast<Item>(currentSelectItem_))
		{
		// ゲームスタート
		case Item::START:
			manager_.ChangeScene(new MainScene(manager_));
			return;

		// オプション
		case Item::OPSITON:
			manager_.PushScene(new OptionScene(manager_));
			break;

		// ゲーム終了
		case Item::END:
			manager_.SetIsGameEnd(true);
			return;

		// ありえないので止める
		default:
			assert(0);
		}

		// PushSceneした場合シーンが残ったままなので
		// フェードインを開始する
		StartFadeIn();
		return;
	}

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

	// モザイクフェードの描画
	DrawGaussFade(fadeDataTable_[currentSelectItem_].isFade);

	// フェードの描画
	DrawFade(fadeDataTable_[currentSelectItem_].isGaussFade);
}