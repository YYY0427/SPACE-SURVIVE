#include "CubeLazer.h"
#include "../Util/Debug.h"

CubeLazer::CubeLazer(int modelHandle)
{
	pModel_ = std::make_unique<Model>(modelHandle);

	pModel_->SetUseCollision(true);

	// ƒ‚ƒfƒ‹‚ÌŠg‘å—¦‚ğ0.00`0.1‚ÌŠÔ‚Åæ“¾
	scale_.x = static_cast<float>(GetRand(10)) * 0.01f;
	scale_.y = static_cast<float>(GetRand(10)) * 0.01f;
	scale_.z = static_cast<float>(GetRand(10)) * 0.01f;

//	scale_ = { 100.0f, 100.0f, 100.0f};

	// 1ƒtƒŒ[ƒ€‚É‰ñ“]‚·‚é—Ê‚ğ0“x‚©‚ç`1“x‚ÌŠÔ‚©‚çæ“¾
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

void CubeLazer::CheckInCamera()
{
	if (CheckCameraViewClip(pos_))
	{
		isEnabled_ = false;
	}
}