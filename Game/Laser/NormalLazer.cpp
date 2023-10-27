#include "NormalLazer.h"
#include "../Util/DataReaderFromUnity.h"
#include "../Util/Debug.h"

NormalLazer::NormalLazer(int modelHandle)
{
	pModel_ = std::make_unique<Model>(modelHandle);
}

NormalLazer::~NormalLazer()
{
}

void NormalLazer::Update()
{
	pos_ = VAdd(pos_, vec_);

	pModel_->SetRot(rot_);
	pModel_->SetPos(pos_);
	pModel_->Update();
}

void NormalLazer::Draw()
{
#ifdef _DEBUG
	pModel_->Draw();
#endif
}