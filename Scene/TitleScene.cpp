#include <DxLib.h>
#include "TitleScene.h"
#include "SceneManager.h"
#include "MainScene.h"
#include "../InputState.h"
#include "../Util/SoundManager.h"
#include "../Util/DrawFunctions.h"
#include "../Game.h"

/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="manager">シーンマネージャーの参照</param>
TitleScene::TitleScene(SceneManager& manager) :
	Scene(manager),
	updateFunc_(&TitleScene::NormalUpdate)
{
}

/// <summary>
/// デストラクタ
/// </summary>
TitleScene::~TitleScene()
{
}

/// <summary>
/// 更新
/// </summary>
void TitleScene::Update(const InputState& input)
{
	(this->*updateFunc_)(input);
}

/// <summary>
/// 通常の更新
/// </summary>
void TitleScene::NormalUpdate(const InputState& input)
{
	if (isFadeOut_ && !IsFadingOut())
	{
		manager_.ChangeScene(new MainScene(manager_));
		return;
	}

	//次へのボタンが押されたら次のシーンへ行く
	if (input.IsTriggered(InputType::next) && !isFadeOut_)
	{
		isFadeOut_ = true;
		StartFadeOut();
	}
	UpdateFade();
}

/// <summary>
/// 描画
/// </summary>
void TitleScene::Draw()
{
	DrawString(0, 0, "TitleScene", 0xffffff, true);

	DrawFade();
}