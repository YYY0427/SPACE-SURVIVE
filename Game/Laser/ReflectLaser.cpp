#include "ReflectLaser.h"
#include "../Util/MathUtil.h"
#include "../Util/Model.h"
#include "../Util/Effekseer3DEffectManager.h"

namespace
{
	constexpr VECTOR init_model_direction{ -1, 0, 0 };
	constexpr VECTOR init_effect_direction{ 0, 0, -1 };
	constexpr VECTOR model_scale = { 1.0f, 0.1f, 0.1f };
	constexpr VECTOR effect_scale = { 30.0f, 30.0f, 24.0f };
}

ReflectLaser::ReflectLaser(int modelHandle, VECTOR pos, VECTOR vec)
{
	pos_ = pos;
	vec_ = vec;
	isEnabled_ = true;

	// �x�N�g�������̉�]�s�񂩂�I�C���[�p���o��
	MATRIX rotEffectMtx = MGetRotVec2(init_effect_direction, VScale(vec_, -1));
	bool isGimbalLock = false;
	VECTOR effectRot = MathUtil::ToEulerAngles(rotEffectMtx, isGimbalLock);

	// �G�t�F�N�g�̍Đ�
	auto& effectManager = Effekseer3DEffectManager::GetInstance();
	effectManager.PlayEffectLoopAndFollow(laserEffectHandle_, EffectID::refrect_laser, &pos_, effect_scale, 1.0f, effectRot);

	// �x�N�g�������̉�]�s����쐬
	rotMtx_ = MGetRotVec2(init_model_direction, vec_);

	// �����蔻��Ɏg�p���郂�f���̐ݒ�
	pModel_ = std::make_unique<Model>(modelHandle);	// �C���X�^���X����
	pModel_->SetUseCollision(true);					// �����蔻��ݒ�
	pModel_->SetScale(model_scale);	// �g�嗦
	pModel_->SetRotMtx(rotMtx_);	// ��]�s��
	pModel_->SetPos(pos_);			// �ʒu
	pModel_->Update();				// �����蔻��̍X�V
}

ReflectLaser::~ReflectLaser()
{
	auto& effectManager = Effekseer3DEffectManager::GetInstance();
	effectManager.DeleteEffect(laserEffectHandle_);
}

void ReflectLaser::Update()
{
}

void ReflectLaser::Draw()
{
#ifdef _DEBUG
	pModel_->Draw();
#endif
}

void ReflectLaser::ReflectLaserUpdate(VECTOR pos, VECTOR vec)
{
	pos_ = pos;
	vec_ = vec;

	// �x�N�g�������̉�]�s�񂩂�I�C���[�p���o��
	MATRIX rotEffectMtx = MGetRotVec2(init_effect_direction, vec_);
	bool isGimbalLock = false;
	VECTOR effectRot = MathUtil::ToEulerAngles(rotEffectMtx, isGimbalLock);
	auto& effectManager = Effekseer3DEffectManager::GetInstance();
	effectManager.SetEffectRot(laserEffectHandle_, effectRot);

	// �x�N�g�������̉�]�s����쐬
	rotMtx_ = MGetRotVec2(init_model_direction, vec_);

	pModel_->SetRotMtx(rotMtx_);	// ��]�s��
	pModel_->SetPos(pos_);			// �ʒu
	pModel_->Update();				// �����蔻��̍X�V
}