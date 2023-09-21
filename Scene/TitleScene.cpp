#include <DxLib.h>
#include "TitleScene.h"
#include "SceneManager.h"
#include "MainScene.h"
#include "DebugScene.h"
#include "../Util/InputState.h"
#include "../Util/SoundManager.h"
#include "../Util/DrawFunctions.h"
#include "../common.h"

// コンストラクタ
TitleScene::TitleScene(SceneManager& manager) :
	Scene(manager),
	updateFunc_(&TitleScene::NormalUpdate)
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
		manager_.ChangeScene(new DebugScene(manager_));
		return;
	}
	// フェードの更新
	UpdateFade();
}

// 描画
void TitleScene::Draw()
{
	// 現在のシーンのテキスト表示
	DrawString(0, 0, "TitleScene", 0xffffff, true);

	// フェードの描画
	DrawFade();
}