#include "MainScene.h"
#include "TitleScene.h"
#include "PauseScene.h"
#include "SceneManager.h"
#include "../Game.h"
#include "../InputState.h"
#include "../Util/Effekseer3DEffectManager.h"
#include "../Util/SoundManager.h"
#include <DxLib.h>

namespace
{
	constexpr int fade_interval = 60;
}

MainScene::MainScene(SceneManager& manager) :
	Scene(manager),
	updateFunc_(&MainScene::FadeInUpdate),
	fadeTimer_(fade_interval)
{
	auto& effect = Effekseer3DEffectManager::GetInstance();
	
	SoundManager::GetInstance().PlayBGM("test");
}

MainScene::~MainScene()
{
}

void MainScene::Update(const InputState& input)
{
	(this->*updateFunc_)(input);
}

void MainScene::Draw()
{
	DrawString(0, 0, "MainScene", 0xffffff, true);

	VECTOR Pos1;
	VECTOR Pos2;
	float LINE_AREA_SIZE = 10000.0f;
	int LINE_NUM = 50;

	SetUseZBufferFlag(TRUE);

	Pos1 = VGet(-LINE_AREA_SIZE / 2.0f, 0.0f, -LINE_AREA_SIZE / 2.0f);
	Pos2 = VGet(-LINE_AREA_SIZE / 2.0f, 0.0f, LINE_AREA_SIZE / 2.0f);
	for (int i = 0; i <= LINE_NUM; i++)
	{
		DrawLine3D(Pos1, Pos2, GetColor(0, 0, 0));
		Pos1.x += LINE_AREA_SIZE / LINE_NUM;
		Pos2.x += LINE_AREA_SIZE / LINE_NUM;
	}

	Pos1 = VGet(-LINE_AREA_SIZE / 2.0f, 0.0f, -LINE_AREA_SIZE / 2.0f);
	Pos2 = VGet(LINE_AREA_SIZE / 2.0f, 0.0f, -LINE_AREA_SIZE / 2.0f);
	for (int i = 0; i < LINE_NUM; i++)
	{
		DrawLine3D(Pos1, Pos2, GetColor(0, 0, 0));
		Pos1.z += LINE_AREA_SIZE / LINE_NUM;
		Pos2.z += LINE_AREA_SIZE / LINE_NUM;
	}

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, fadeValue_);
	DrawBox(0, 0, Game::screen_width, Game::screen_height, 0xffffff, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void MainScene::FadeInUpdate(const InputState& input)
{
	fadeValue_ = static_cast<int>(255 * static_cast<float>(fadeTimer_) / static_cast<float>(fade_interval));
	if (--fadeTimer_ == 0)
	{
		updateFunc_ = &MainScene::NormalUpdate;
	}
}

void MainScene::NormalUpdate(const InputState& input)
{
	

	if (input.IsTriggered(InputType::next))
	{
		updateFunc_ = &MainScene::FadeOutUpdate;
	}
	if (input.IsTriggered(InputType::pause))
	{
		manager_.PushScene(new PauseScene(manager_));
	}
}

void MainScene::FadeOutUpdate(const InputState& input)
{
	fadeValue_ = static_cast<int>(255 * static_cast<float>(fadeTimer_) / static_cast<float>(fade_interval));
	if (++fadeTimer_ == fade_interval)
	{
		manager_.ChangeScene(new TitleScene(manager_));
		return;
	}
}