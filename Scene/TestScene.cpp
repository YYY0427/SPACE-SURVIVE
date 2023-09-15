#include "TestScene.h"
#include "SceneManager.h"
#include "DebugScene.h"
#include "../Util/Effekseer3DEffectManager.h"
#include "../Util/SoundManager.h"
#include "../Util/InputState.h"

/// <summary>
/// コンストラクタ
/// </summary>
TestScene::TestScene(SceneManager& manager) :
	Scene(manager)
{
	auto& effect = Effekseer3DEffectManager::GetInstance();
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
	// フェードアウトが終わり次第シーン遷移
	if (isFadeOut_ && !IsFadingOut())
	{
		manager_.ChangeScene(new DebugScene(manager_));
		return;
	}

	static int j = 0;
	if (input.IsXInputStick(XInputType::LEFT) == XInputTypeStick::LITTLE_LEFT)
	{
		j = 1;
	}
	else if (input.IsXInputStick(XInputType::LEFT) == XInputTypeStick::LEFT)
	{
		j = 10;
	}
	else
	{
		j = 0;
	}
	DrawFormatString(200, 200, 0xffffff, "%d", j);

	// 戻るボタンが押されてフェードインしてなかったらフェードアウト開始
	if (input.IsTriggered(InputType::BACK) && !IsFadingIn())
	{
		StartFadeOut();

		// フェードの設定の変更
		SetFadeConfig(3, VGet(255, 255, 255), GetFadeBright());

		isFadeOut_ = true;
	}
	// フェードの更新
	UpdateFade();
}

/// <summary>
/// 描画
/// </summary>
void TestScene::Draw()
{
	DrawString(0, 0, "TestScene", 0xffffff, true);

	VECTOR pos1;
	VECTOR pos2;
	float lineAreaSize = 10000.0f;
	int lineNum = 50;

	pos1 = VGet(-lineAreaSize / 2.0f, 0.0f, -lineAreaSize / 2.0f);
	pos2 = VGet(-lineAreaSize / 2.0f, 0.0f, lineAreaSize / 2.0f);
	for (int i = 0; i <= lineNum; i++)
	{
		DrawLine3D(pos1, pos2, GetColor(0, 0, 0));
		pos1.x += lineAreaSize / lineNum;
		pos2.x += lineAreaSize / lineNum;
	}

	pos1 = VGet(-lineAreaSize / 2.0f, 0.0f, -lineAreaSize / 2.0f);
	pos2 = VGet(lineAreaSize / 2.0f, 0.0f, -lineAreaSize / 2.0f);
	for (int i = 0; i < lineNum; i++)
	{
		DrawLine3D(pos1, pos2, GetColor(0, 0, 0));
		pos1.z += lineAreaSize / lineNum;
		pos2.z += lineAreaSize / lineNum;
	}

	// フェードの描画
	DrawFade();
}