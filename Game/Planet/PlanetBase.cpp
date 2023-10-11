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

// �`��
void PlanetBase::Draw()
{
	pModel_->Draw();
}

// ���f���n���h���̎擾
int PlanetBase::GetModelHandle() const
{
	return pModel_->GetModelHandle();
}