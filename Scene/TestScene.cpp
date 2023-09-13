#include "TestScene.h"
#include "SceneManager.h"
#include "DebugScene.h"
#include "../Util/Effekseer3DEffectManager.h"
#include "../Util/SoundManager.h"
#include "../InputState.h"

/// <summary>
/// コンストラクタ
/// </summary>
TestScene::TestScene(SceneManager& manager) :
	Scene(manager)
{
	auto& effect = Effekseer3DEffectManager::GetInstance();
	SoundManager::GetInstance().PlayBGM("test");
}

/// <summary>
///  デストラクタ
/// </summary>
TestScene::~TestScene()
{
}

/// <summary>
/// 更新
/// </summary>
void TestScene::Update(const InputState& input)
{
	if (isFadeOut_ && !IsFadingOut())
	{
		manager_.ChangeScene(new DebugScene(manager_));
		return;
	}

	if (input.IsTriggered(InputType::next) && !IsFadingIn())
	{
		StartFadeOut();
		SetFadeConfig(1, VGet(255, 255, 255), GetFadeBright());
		isFadeOut_ = true;
	}
	UpdateFade();
}

/// <summary>
/// 描画
/// </summary>
void TestScene::Draw()
{
	DrawString(0, 0, "TestScene", 0xffffff, true);
	DrawFade();
}