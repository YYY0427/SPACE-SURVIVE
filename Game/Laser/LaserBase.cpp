#include "LaserBase.h"
#include "../Util/Effekseer3DEffectManager.h"

LaserBase::LaserBase():
	pos_({}),
	vec_({}),
	rot_({}),
	scale_({}),
	firePos_(nullptr),
	isEnabled_(true),
	isReflect_(false)
{
}

LaserBase::~LaserBase()
{
}

bool LaserBase::IsEnabled() const
{
	return isEnabled_;
}

int LaserBase::GetModelHandle() const
{
	return pModel_->GetModelHandle();
}

VECTOR LaserBase::GetVec() const
{
	return vec_;
}

bool LaserBase::IsReflect() const
{
	return isReflect_;
}

void LaserBase::SetIsReflect(bool isReflect)
{
	isReflect_ = isReflect;
}