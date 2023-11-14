#include <DxLib.h>
#include <cassert>
#include "TitleScene.h"
#include "SceneManager.h"
#include "OptionScene.h"
#include "DebugScene.h"
#include "GameMainScene.h"
#include "../Util/InputState.h"
#include "../Util/SoundManager.h"
#include "../Util/DrawFunctions.h"
#include "../Util/StringManager.h"
#include "../Util/Debug.h"
#include "../common.h"

namespace
{
	// タイトルの背景画像のファイルパス
	const std::string background_file_path = "Data/Image/Background.png";

	// タイトルロゴの画像のファイルパス
	const std::string title_logo_file_path = "Data/Image/TitleLogo.png";

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
	backGroundHandle_ = my::MyLoadGraph(background_file_path.c_str());
	titleLogoHandle_ = my::MyLoadGraph(title_logo_file_path.c_str());

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
	DeleteGraph(backGroundHandle_);

	SoundManager::GetInstance().StopSound("bgmTest");
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
			manager_.ChangeScene(new GameMainScene(manager_));
			return;

		// オプション
		case Item::OPSITON:
			manager_.PushScene(new OptionScene(manager_));
			break;

		// ゲーム終了
		case Item::END:
#ifdef _DEBUG
			manager_.ChangeScene(new DebugScene(manager_));
			return;
#else 
			manager_.SetIsGameEnd(true);
#endif
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
	// 背景の描画
	DrawRotaGraph(common::screen_width / 2, common::screen_height / 2, 1.0, 0.0, backGroundHandle_, true);

	// タイトルのロゴの描画
	DrawRotaGraph(common::screen_width / 2, common::screen_height / 2 - 150, 0.3, 0.0, titleLogoHandle_, true);

	// 現在のシーンのテキスト描画
	Debug::Log("TitleScne");

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