#include "PlanetBase.h"
#include "../Util/Model.h"

PlanetBase::PlanetBase() :
	pos_(VGet(0.0f, 0.0f, 0.0f)),
	rot_(VGet(0.0f, 0.0f, 0.0f))
{
}

PlanetBase::~PlanetBase()
{
}

// •`‰æ
void PlanetBase::Draw()
{
	pModel_->Draw();
}

// ƒ‚ƒfƒ‹ƒnƒ“ƒhƒ‹‚ÌŽæ“¾
int PlanetBase::GetModelHandle() const
{
	return pModel_->GetModelHandle();
}