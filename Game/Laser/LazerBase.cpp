#include "LazerBase.h"

LazerBase::LazerBase():
	pos_({}),
	vec_({}),
	rot_({}),
	scale_({}),
	isEnabled_(false),
	modelHandle_(-1)
{
}

LazerBase::~LazerBase()
{
}

void LazerBase::Fire(const VECTOR pos, const VECTOR vec)
{
	pos_ = pos;
	vec_ = vec;
	isEnabled_ = true;
}

void LazerBase::Refrect()
{
	vec_ = VScale(vec_, -1);
}

bool LazerBase::GetIsEnabled() const
{
	return isEnabled_;
}
