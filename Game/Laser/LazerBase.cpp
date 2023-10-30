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

void LazerBase::Fire(const VECTOR pos, const VECTOR vec,const VECTOR rot)
{
	pos_ = pos;
	firePos_ = pos;
	rot_ = rot;
	vec_ = vec;
	isEnabled_ = true;
}

void LazerBase::Delete()
{
	isEnabled_ = false;
}

bool LazerBase::GetIsEnabled() const
{
	return isEnabled_;
}

int LazerBase::GetModelHandle() const
{
	return pModel_->GetModelHandle();
}