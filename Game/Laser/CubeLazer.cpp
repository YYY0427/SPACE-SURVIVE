#include "CubeLazer.h"
#include "../Util/Debug.h"

CubeLazer::CubeLazer(int modelHandle)
{
	pModel_ = std::make_unique<Model>(modelHandle);
	scale_ = { 0.1f, 0.1f, 0.1f };
}

CubeLazer::~CubeLazer()
{
}

void CubeLazer::Update()
{
	pos_ = VAdd(pos_, vec_);

	pModel_->SetScale(scale_);
	pModel_->SetRot(rot_);
	pModel_->SetPos(pos_);
	pModel_->Update();

	Debug::Log("レーザー位置", pos_);
}

void CubeLazer::Draw()
{
	pModel_->Draw();
}