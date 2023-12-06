#include "ScreenEffect.h"
#include <DxLib.h>

ScreenEffect::ScreenEffect() :
	quakeX_(0.0f),
	quakeY_(0.0f)
{
	frame_.SetTime(0);
}

ScreenEffect::~ScreenEffect()
{
}

void ScreenEffect::Update()
{
	if (frame_.GetTime() > 0)
	{
		// ‰æ–Ê‚ð—h‚ç‚·
		quakeX_ *= -0.95f;
		quakeY_ *= -0.95f;
		frame_.Update(-1);
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
}

void ScreenEffect::SetShake(float quakeXSize, float quakeYSize, int frame)
{
	if (frame_.GetTime() > 0)	return;

	quakeX_ = quakeXSize;
	quakeY_ = quakeYSize;
	frame_.SetTime(frame);	
}
