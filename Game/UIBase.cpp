#include "UIBase.h"

UIBase::UIBase()
{
}

UIBase::~UIBase()
{
}

void UIBase::Store(Vector2 vec, float speed)
{
	vec.Normalize();
	vec *= speed;
	pos_ += vec;
}