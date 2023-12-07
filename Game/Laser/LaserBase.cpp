#include "LaserBase.h"
#include "../Util/Effekseer3DEffectManager.h"

LaserBase::LaserBase():
	pos_({}),
	vec_({}),
	rot_({}),
	scale_({}),
	firePos_(nullptr),
	isEnabled_(true),
	isReflect_(false),
	alpha_(1.0f)
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

void LaserBase::GraduallyAlphaDelete()
{
	alpha_ -= 0.05f;
	if (alpha_ < 0.0f)
	{
		isEnabled_ = false;
	}
	pModel_->SetOpacity(alpha_);
}

bool LaserBase::IsReflect() const
{
	return isReflect_;
}

void LaserBase::SetIsReflect(bool isReflect)
{
	isReflect_ = isReflect;
}