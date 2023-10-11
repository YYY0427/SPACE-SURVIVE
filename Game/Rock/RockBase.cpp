#include "RockBase.h"
#include "../Util/Model.h"

RockBase::RockBase() :
    isEnabled_(false),
    pos_(VGet(0.0f, 0.0f, 0.0f)),
    rot_(VGet(0.0f, 0.0f, 0.0f)),
    vec_(VGet(0.0f, 0.0f, 0.0f))
{
}

RockBase::~RockBase()
{
}

void RockBase::Draw()
{
    pModel_->Draw();
}

int RockBase::GetModelHandle() const
{
    return pModel_->GetModelHandle();
}

bool RockBase::GetIsEnabled() const
{
    return isEnabled_;
}
