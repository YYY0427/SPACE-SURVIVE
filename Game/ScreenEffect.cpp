#include "ScreenEffect.h"
#include "common.h"
#include <DxLib.h>

ScreenEffect::ScreenEffect() :
	quakeX_(0.0f),
	quakeY_(0.0f),
	alpha_(0),
	speed_(0),
	color_(0x000000)
{
	quakeFrame_.SetTime(0);
}

ScreenEffect::~ScreenEffect()
{
}

void ScreenEffect::Update()
{
	alpha_ -= speed_;
	alpha_ = (std::max)(alpha_, 0);

	// —h‚ê‚Ìˆ—
	if (quakeFrame_.GetTime() > 0)
	{
		// ‰æ–Ê‚ð—h‚ç‚·
		quakeX_ *= -0.95f;
		quakeY_ *= -0.95f;
		quakeFrame_.Update(-1);
	}
	else
	{
		quakeX_ = 0.0f;
		quakeY_ = 0.0f;
	}
}

void ScreenEffect::Draw(int screenHandle)
{
	DrawGraph(static_cast<int>(quakeX_), static_cast<int>(quakeY_), screenHandle, false);

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha_);
	DrawBox(0, 0, common::screen_width, common::screen_height, color_, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void ScreenEffect::SetShake(float quakeXSize, float quakeYSize, int frame)
{
	if (quakeFrame_.GetTime() > 0)	return;

	quakeX_ = quakeXSize;
	quakeY_ = quakeYSize;
	quakeFrame_.SetTime(frame);
}

void ScreenEffect::SetScreenColor(unsigned int color, int alpha, int speed)
{
	color_ = color;
	alpha_ = alpha;
	speed_ = speed;
}
