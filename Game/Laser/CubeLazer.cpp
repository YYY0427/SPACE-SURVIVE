#include "CubeLazer.h"
#include "../Util/Debug.h"

namespace
{
	constexpr float expansion_speed = 0.0001f;
}

CubeLazer::CubeLazer(int modelHandle, VECTOR pos, VECTOR vec)
{
	pos_ = pos;
	vec_ = vec;
	isEnabled_ = true;

	pModel_ = std::make_unique<Model>(modelHandle);

	pModel_->SetUseCollision(true);

	// ���f���̊g�嗦��0.00�`0.1�̊ԂŎ擾
	endScale_.x = static_cast<float>(GetRand(10)) * 0.01f;
	endScale_.y = static_cast<float>(GetRand(10)) * 0.01f;
	endScale_.z = static_cast<float>(GetRand(10)) * 0.01f;

	// 1�t���[���ɉ�]����ʂ�0�x����`1�x�̊Ԃ���擾
	deltaRot_ = 1.0f * (static_cast<float>(GetRand(10)) * 0.1f);
	deltaRot_ = deltaRot_ * DX_PI_F / 180.0f;

	pModel_->SetPos(pos_);
	pModel_->Update();
}

CubeLazer::~CubeLazer()
{
}

void CubeLazer::Update()
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
	//	isEnabled_ = false;
	}
}