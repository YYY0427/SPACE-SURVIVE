#include "MainScene.h"
#include "TitleScene.h"
#include "PauseScene.h"
#include "SceneManager.h"
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
		// フェードアウト開始
		StartFadeOut();
	}
	// フェードアウトが終わり次第シーン遷移
	if (IsStartFadeOutAfterFadingOut())
	{
		StartFadeIn();
		manager_.PushScene(new PauseScene(manager_));
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

	// モザイクフェードの描画
	DrawGaussFade();
}