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
void MainScene::Update()
{
	(this->*updateFunc_)();
}

/// <summary>
/// 通常の更新
/// </summary>
void MainScene::NormalUpdate()
{
	// フェードが終わり次第シーン遷移
	if (isFadeOut_ && !IsFadingOut())
	{
		manager_.ChangeScene(new DebugScene(manager_));
		return;
	}

	// 戻るボタンが押されたらフェードアウト開始
	if (InputState::IsTriggered(InputType::BACK) && !IsFadingIn())
	{
		StartFadeOut();
		isFadeOut_ = true;
	}
	// ポーズ画面に遷移
	if (InputState::IsTriggered(InputType::PAUSE))
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

	{
		DINPUT_JOYSTATE input;
		// 入力状態を取得
		GetJoypadDirectInputState(DX_INPUT_PAD1, &input);


		static int vol = 0;
		if (input.Rx < -1.0f)
		{
			// 0から1000の範囲を0から10に
			vol = ((input.Rx - 0) * (10 - 0)) / (-1000 - 0);
		}
		else if (input.Rx > 1.0f)
		{
			// 0から1000の範囲を0から10に
			vol = ((input.Rx - 0) * (10 - 0)) / (1000 - 0);
		}
		else if (input.Ry < -1.0f)
		{
			// 0から1000の範囲を0から10に
		//	vol = ((input.Ry - 0) * (10 - 0)) / (-1000 - 0);
			vol = input.Ry;
		}
		else if (input.Ry > 1.0f)
		{
			// 0から1000の範囲を0から10に
			vol = ((input.Ry - 0) * (10 - 0)) / (1000 - 0);
		}
		else
		{
			vol = 0;
		}
		DrawFormatString(300, 300, 0xff0000, "スティック%d", vol);
	}

	// フェードの描画
	DrawFade();
}