#include <DxLib.h>
#include "TitleScene.h"
#include "SceneManager.h"
#include "MainScene.h"
#include "../InputState.h"
#include "../Util/SoundManager.h"
#include "../Util/DrawFunctions.h"
#include "../Game.h"

namespace
{
	constexpr int fade_interval = 60;
}

void TitleScene::FadeInUpdate(const InputState& input)
{
	fadeValue_ = static_cast<int>(255 * (static_cast<float>(fadeTimer_) / 
		static_cast<float>(fade_interval)));
	if (--fadeTimer_ == 0)
	{
		updateFunc_ = &TitleScene::NormalUpdate;
		fadeValue_ = 0;
	}
}

void TitleScene::NormalUpdate(const InputState& input)
{
//	UpdateFade();
	//���ւ̃{�^���������ꂽ�玟�̃V�[���֍s��
	if (input.IsTriggered(InputType::next))
	{
		updateFunc_ = &TitleScene::FadeOutUpdate;
	}
}

void TitleScene::FadeOutUpdate(const InputState& input)
{
	fadeValue_ = static_cast<int>(255 * (static_cast<float>(fadeTimer_) / static_cast<float>(fade_interval)));
	if (++fadeTimer_ == fade_interval)
	{
		manager_.ChangeScene(new MainScene(manager_));
		return;
	}
}

TitleScene::TitleScene(SceneManager& manager) :
	Scene(manager),
	updateFunc_(&TitleScene::FadeInUpdate),
	fadeTimer_(fade_interval)
{
}

TitleScene::~TitleScene()
{
}

void TitleScene::Update(const InputState& input)
{
	(this->*updateFunc_)(input);
}

void TitleScene::Draw()
{
	DrawString(0, 0, "TitleScene", 0xffffff, true);

//	DrawFade();

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, fadeValue_);
	DrawBox(0, 0, Game::screen_width, Game::screen_height, 0xffffff, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}