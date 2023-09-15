#include "MainScene.h"
#include "TitleScene.h"
#include "PauseScene.h"
#include "SceneManager.h"
#include "DebugScene.h"
#include "../Game.h"
#include "../Util/InputState.h"
#include "../Util/Effekseer3DEffectManager.h"
#include "../Util/SoundManager.h"
#include <DxLib.h>

/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="manager">シーンマネージャーへの参照</param>
MainScene::MainScene(SceneManager& manager) :
	Scene(manager),
	updateFunc_(&MainScene::NormalUpdate)
{
	
}

/// <summary>
/// デストラクタ
/// </summary>
MainScene::~MainScene()
{
}

/// <summary>
/// 更新
/// </summary>
void MainScene::Update(const InputState& input)
{
	(this->*updateFunc_)(input);
}

/// <summary>
/// 通常の更新
/// </summary>
void MainScene::NormalUpdate(const InputState& input)
{
	// フェードが終わり次第シーン遷移
	if (isFadeOut_ && !IsFadingOut())
	{
		manager_.ChangeScene(new DebugScene(manager_));
		return;
	}

	// 戻るボタンが押されたらフェードアウト開始
	if (input.IsTriggered(InputType::BACK) && !IsFadingIn())
	{
		StartFadeOut();
		isFadeOut_ = true;
	}
	// ポーズ画面に遷移
	if (input.IsTriggered(InputType::PAUSE))
	{
		manager_.PushScene(new PauseScene(manager_));
		return;
	}
	// フェードの更新
	UpdateFade();
}

/// <summary>
/// 描画
/// </summary>
void MainScene::Draw()
{
	DrawString(0, 0, "MainScene", 0xffffff, true);

	// フェードの描画
	DrawFade();
}