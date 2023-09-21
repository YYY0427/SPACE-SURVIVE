#include "MainScene.h"
#include "TitleScene.h"
#include "PauseScene.h"
#include "SceneManager.h"
#include "DebugScene.h"
#include "../common.h"
#include "../Util/InputState.h"
#include "../Util/Effekseer3DEffectManager.h"
#include "../Util/SoundManager.h"
#include <DxLib.h>

// コンストラクタ
MainScene::MainScene(SceneManager& manager) :
	Scene(manager),
	updateFunc_(&MainScene::NormalUpdate)
{
	
}

// デストラクタ
MainScene::~MainScene()
{
	// 処理なし
}

// メンバ関数ポインタの更新
void MainScene::Update()
{
	(this->*updateFunc_)();
}

// 通常の更新
void MainScene::NormalUpdate()
{
	
	// ポーズ画面に遷移
	if (InputState::IsTriggered(InputType::PAUSE))
	{
		manager_.PushScene(new PauseScene(manager_));
		return;
	}
	// 戻るボタンが押され、フェード中じゃない場合フェードアウト開始
	if (InputState::IsTriggered(InputType::BACK) && !IsFadeing())
	{
		// フェードアウト開始
		StartFadeOut();

		// フェードアウトが行われたかどうかのフラグを立てる
		// シーン遷移の際、フェードアウトが行われたかどうかを確認するため
		isFadeOut_ = true;
	}
	// フェードアウトが終わり次第シーン遷移
	if (isFadeOut_ && !IsFadingOut())
	{
		manager_.ChangeScene(new DebugScene(manager_));
		return;
	}
	// フェードの更新
	UpdateFade();
}

// 描画
void MainScene::Draw()
{
	// 現在のシーンのテキスト表示
	DrawString(0, 0, "MainScene", 0xffffff, true);

	// フェードの描画
	DrawFade();
}