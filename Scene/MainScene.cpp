#include "MainScene.h"
#include "TitleScene.h"
#include "PauseScene.h"
#include "SceneManager.h"
#include "../Game.h"
#include "../InputState.h"
#include "../Util/Effekseer3DEffectManager.h"
#include "../Util/SoundManager.h"
#include <DxLib.h>

MainScene::MainScene(SceneManager& manager) :
	Scene(manager),
	updateFunc_(&MainScene::NormalUpdate)
{
	auto& effect = Effekseer3DEffectManager::GetInstance();
	SetFadeConfig(8, VGet(255, 255, 255), GetFadeBright());
	SoundManager::GetInstance().PlayBGM("test");
}

MainScene::~MainScene()
{
}

void MainScene::Update(const InputState& input)
{
	(this->*updateFunc_)(input);
}

void MainScene::NormalUpdate(const InputState& input)
{
	if (isFadeOut_ && !IsFadingOut())
	{
		manager_.ChangeScene(new TitleScene(manager_));
		return;
	}


	if (input.IsTriggered(InputType::next) && !IsFadingIn())
	{
		StartFadeOut();
		SetFadeConfig(1, VGet(255, 255, 255), GetFadeBright());
		isFadeOut_ = true;
	}
	if (input.IsTriggered(InputType::pause))
	{
		manager_.PushScene(new PauseScene(manager_));
	}
	UpdateFade();
}

void MainScene::Draw()
{
	DrawString(0, 0, "MainScene", 0xffffff, true);

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
	DrawFade();
}