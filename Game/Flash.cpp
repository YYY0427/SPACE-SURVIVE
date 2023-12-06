#include "Flash.h"
#include <DxLib.h>

namespace
{
	// 拡大スピード
	constexpr float scale_speed = 100.0f;
}

Flash::Flash(int enableFrame):
	pos_(0.0f, 0.0f),
	enableFrame_(enableFrame),
	color_(0),
	radius_(200.0f),
	alpha_(255)
{
}

Flash::~Flash()
{
}

void Flash::Update(Vector2 pos, unsigned int color)
{
	pos_ = pos;
	color_ = color;
	radius_ += scale_speed;

	enableFrame_.Update(1);
	if (enableFrame_.IsTimeOut())
	{
		alpha_ -= 10;
		alpha_ = (std::max)(alpha_, 0);
	}
}

void Flash::Draw()
{
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha_);
	DrawCircle(pos_.x, pos_.y, radius_, color_, TRUE);	
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

bool Flash::IsEnd() const
{
	return alpha_ <= 0;
}

int Flash::GetAlpha() const
{
	return alpha_;
}