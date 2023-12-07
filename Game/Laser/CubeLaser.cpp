#include "CubeLaser.h"
#include "../Util/Debug.h"

namespace
{
	// 拡大速度
	constexpr float expansion_speed = 0.0002f;
}

CubeLaser::CubeLaser(int modelHandle, VECTOR pos, VECTOR vec)
{
	pos_ = pos;
	vec_ = vec;
	isEnabled_ = true;

	pModel_ = std::make_unique<Model>(modelHandle);
	pModel_->SetUseCollision(true);

	// モデルの拡大率を0.05～0.51の間で取得
	endScale_.x = static_cast<float>(GetRand(50) + 5) * 0.01f;
	endScale_.y = static_cast<float>(GetRand(50) + 5) * 0.01f;
	endScale_.z = static_cast<float>(GetRand(50) + 5) * 0.01f;

	// 1フレームに回転する量を0度から～1度の間から取得
	deltaRot_ = 1.0f * (static_cast<float>(GetRand(10)) * 0.1f);
	deltaRot_ = deltaRot_ * DX_PI_F / 180.0f;

	pModel_->SetOpacity(alpha_);	// 不透明度
	pModel_->SetPos(pos_);
	pModel_->Update();
}

CubeLaser::~CubeLaser()
{
}

void CubeLaser::Update()
{
	if (scale_.x <= endScale_.x)
	{
		scale_.x += expansion_speed;
	}
	if (scale_.y <= endScale_.y)
	{
		scale_.y += expansion_speed;
	}
	if (scale_.z <= endScale_.z)
	{
		scale_.z += expansion_speed;
	}

	rot_ = VAdd(rot_, { deltaRot_, deltaRot_, deltaRot_ });
	pos_ = VAdd(pos_, vec_);

	pModel_->SetScale(scale_);
	pModel_->SetRot(rot_);
	pModel_->SetPos(pos_);
	pModel_->SetOpacity(alpha_);	// 不透明度
	pModel_->Update();
}

void CubeLaser::Draw()
{
	pModel_->Draw();
}

void CubeLaser::ConfirmDelete()
{
}