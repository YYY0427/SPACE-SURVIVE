#include "Scene.h"
#include "../Game.h"
#include <DxLib.h>

Scene::Scene(SceneManager& manager) :
	manager_(manager)
{
	fadeColor_ = GetColor(255, 255, 255);
	fadeBright_ = 255;
	fadeSpeed_ = -8;
}

void Scene::UpdateFade()
{
	fadeBright_ += fadeSpeed_;
	if (fadeBright_ >= 255)
	{
		fadeBright_ = 255;
		if (fadeSpeed_ > 0)
		{
			fadeSpeed_ = 0;
		}
	}
	if (fadeBright_ <= 0)
	{
		fadeBright_ = 0;
		if (fadeSpeed_ < 0)
		{
			fadeSpeed_ = 0;
		}
	}
}

void Scene::DrawFade()
{
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, fadeBright_);
	DrawBox(0, 0, Game::screen_width, Game::screen_height, fadeColor_, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}
