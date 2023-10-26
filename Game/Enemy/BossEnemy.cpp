#include "BossEnemy.h"
#include "../Util/Model.h"

BossEnemy::BossEnemy(int modelHandle)
{
	pModel_ = std::make_unique<Model>(modelHandle);

}

BossEnemy::~BossEnemy()
{
}

void BossEnemy::Update()
{
	pModel_->SetRot(rot_);
	pModel_->SetPos(pos_);
	pModel_->Update();
}

void BossEnemy::Draw()
{
	pModel_->Draw();
}
