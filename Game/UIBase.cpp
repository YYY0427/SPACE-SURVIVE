#include "UIBase.h"
#include "common.h"

UIBase::UIBase() :
	isEnabled_(true)
{
}

UIBase::~UIBase()
{
}

void UIBase::Store(Vector2 vec)
{
	pos_ += vec;

	/*if (pos_.x < 0 || pos_.x > common::screen_width ||
		pos_.y < 0 || pos_.y > common::screen_height)
	{
		isEnabled_ = false;
	}*/
}

bool UIBase::IsEnabled() const
{
	return isEnabled_;
}
