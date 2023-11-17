#include "NormalLazer.h"
#include "../Util/DataReaderFromUnity.h"
#include "../Util/Effekseer3DEffectManager.h"
#include "../Util/Debug.h"
#include "../MathUtil.h"

namespace
{
	constexpr VECTOR model_scale = { 7.0f, 0.5f, 0.5f };
	constexpr VECTOR init_model_direction{ 1, 0, 0 };
	constexpr VECTOR init_effect_direction{ 0, 0, -1 };
	constexpr float effect_scale = 150.0f;

	constexpr int collision_and_effect_difference_frame = 120;
}

NormalLazer::NormalLazer(int modelHandle, VECTOR* pos, VECTOR* vec, VECTOR* enemyMoveVec) :
	collisionAndEffectDifferenceTimer_(collision_and_effect_difference_frame),
	effectPos_({})
{
	// ���f���̃C���X�^���X�̍쐬
	pModel_ = std::make_unique<Model>(modelHandle);

	// �����蔻��ݒ�
	pModel_->SetUseCollision(true);

	// ���f���̊g�嗦�̐ݒ�
	pModel_->SetScale(model_scale);

	firePos_ = pos;
	pos_ = *pos;
	vec_ = vec;
	isEnabled_ = true;
	enemyMoveVec_ = enemyMoveVec;

	// �x�N�g�������̉�]�s������f���ɐݒ�
	MATRIX rotMtx = MGetRotVec2(init_model_direction, *vec_);
	MV1SetRotationMatrix(pModel_->GetModelHandle(), rotMtx);

	// �x�N�g�������̉�]�s�񂩂�I�C���[�p���o��
	MATRIX rotEffectMtx = MGetRotVec2(init_effect_direction, *vec_);
	bool isGimbalLock = false;
	VECTOR effectRot = MathUtil::ToEulerAngles(rotEffectMtx, isGimbalLock);

	// ���[�U�[�̃G�t�F�N�g�̍Đ�
	auto& effectManager = Effekseer3DEffectManager::GetInstance();
	effectManager.PlayEffectFollow(lazerEffectHandle_, EffectID::normal_lazer, firePos_, effect_scale, 1.0f, effectRot);

	pModel_->SetPos(pos_);

	pModel_->Update();
}

NormalLazer::~NormalLazer()
{
}

void NormalLazer::Update()
{
	pos_ = VAdd(pos_, *enemyMoveVec_);

	collisionAndEffectDifferenceTimer_.Update(1);
	if (collisionAndEffectDifferenceTimer_.IsTimeOut())
	{
		pos_ = VAdd(pos_, a_);
	}
	else
	{
		a_ = *vec_;

		// �x�N�g�������̉�]�s������f���ɐݒ�
		MATRIX rotMtx = MGetRotVec2(init_model_direction, *vec_);
		MV1SetRotationMatrix(pModel_->GetModelHandle(), rotMtx);

		// �x�N�g�������̉�]�s�񂩂�I�C���[�p���o��
		MATRIX rotEffectMtx = MGetRotVec2(init_effect_direction, *vec_);
		bool isGimbalLock = false;
		VECTOR effectRot = MathUtil::ToEulerAngles(rotEffectMtx, isGimbalLock);

		auto& effectManager = Effekseer3DEffectManager::GetInstance();
		effectManager.SetEffectRot(lazerEffectHandle_, effectRot);
	}

	pModel_->SetPos(pos_);
	pModel_->Update();
}

void NormalLazer::Draw()
{
#ifdef _DEBUG
	pModel_->Draw();
#endif
}

void NormalLazer::Refrect(const VECTOR pos, const VECTOR norm)
{
	// ���[�U�[�̃G�t�F�N�g���X�g�b�v
	auto& effectManager = Effekseer3DEffectManager::GetInstance();
	effectManager.DeleteEffect(lazerEffectHandle_);

	isRefrect_ = true;
	pos_ = pos;
	effectPos_ = pos;
	pModel_->SetPos(pos);

#if true
	// ���˃x�N�g���̍쐬
	VECTOR inversionVec = VScale(*vec_, -1);
	float dot = VDot(inversionVec, norm);
	dot *= 2.0f;
	VECTOR normVec = VScale(norm, dot);
	*vec_ = VAdd(*vec_, normVec);
	*vec_ = VNorm(*vec_);
	*vec_ = VScale(*vec_, 300.0f);
#else
	vec_ = VScale(vec_, -1);
#endif

	// �x�N�g�������̉�]�s������f���ɐݒ�
	MATRIX rotModelMtx = MGetRotVec2(init_model_direction, *vec_);
	MV1SetRotationMatrix(pModel_->GetModelHandle(), rotModelMtx);

	// �x�N�g�������̉�]�s�񂩂�I�C���[�p���o��
	MATRIX rotEffectMtx = MGetRotVec2(init_effect_direction, *vec_);
	bool isGimbalLock = false;
	VECTOR effectRot = MathUtil::ToEulerAngles(rotEffectMtx, isGimbalLock);

	// ���[�U�[�̃G�t�F�N�g�̍Đ�
	effectManager.PlayEffectFollow(lazerEffectHandle_, EffectID::refrect_laser, &effectPos_, effect_scale, 1.0f, effectRot);

	pModel_->Update();
}

void NormalLazer::CheckInCamera()
{
	if (CheckCameraViewClip(pos_))
	{
//		isEnabled_ = false;
	}
}
