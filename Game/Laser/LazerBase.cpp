#include "LazerBase.h"
#include "../Util/Effekseer3DEffectManager.h"

LazerBase::LazerBase():
	pos_({}),
	vec_({}),
	rot_({}),
	scale_({}),
	isEnabled_(false)
{
}

LazerBase::~LazerBase()
{
}

void LazerBase::Delete()
{
	isEnabled_ = false;
}

bool LazerBase::IsEnabled() const
{
	return isEnabled_;
}

int LazerBase::GetModelHandle() const
{
	return pModel_->GetModelHandle();
}

VECTOR LazerBase::GetVec() const
{
	return vec_;
}
