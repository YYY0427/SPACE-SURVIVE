#include "CubeLazer.h"
#include "../Util/Debug.h"

CubeLazer::CubeLazer(int modelHandle)
{
	pModel_ = std::make_unique<Model>(modelHandle);

	// モデルの拡大率を0.00〜0.1の間で取得
	scale_.x = static_cast<float>(GetRand(10)) * 0.01f;
	scale_.y = static_cast<float>(GetRand(10)) * 0.01f;
	scale_.z = static_cast<float>(GetRand(10)) * 0.01f;

	// 1フレームに回転する量を0度から〜1度の間から取得
	deltaRot_ = 1.0f * (static_cast<float>(GetRand(10)) * 0.1f);
	deltaRot_ = deltaRot_ * DX_PI_F / 180.0f;
}

CubeLazer::~CubeLazer()
{
}

void CubeLazer::Update()
{
	rot_ = VAdd(rot_, { deltaRot_, deltaRot_, deltaRot_ });
	pos_ = VAdd(pos_, vec_);

	pModel_->SetScale(scale_);
	pModel_->SetRot(rot_);
	pModel_->SetPos(pos_);
	pModel_->Update();
}

void CubeLazer::Draw()
{
	pModel_->Draw();
}