#include "NormalLazer.h"
#include "../Util/Effekseer3DEffectManager.h"
#include "../Util/Debug.h"
#include "../Util/MathUtil.h"

namespace
{
	constexpr VECTOR model_scale = { 0.1f, 0.1f, 0.1f };
	constexpr VECTOR init_model_direction{ 1, 0, 0 };
	constexpr VECTOR init_effect_direction{ 0, 0, -1 };
	constexpr VECTOR effect_scale = { 24.0f, 24.0f, 24.0f };

	// �G�t�F�N�g�̃`���[�W���ԃt���[��
	constexpr int effect_charge_frame = 150;
}

NormalLazer::NormalLazer(int modelHandle, VECTOR* firePos, VECTOR* vec, bool isContinue) :
	collisionAndEffectDifferenceTimer_(effect_charge_frame),
	effectPos_({})
{
	// TODO : ���[�U�[���v���C���[��Ǐ]����悤�ɂ���
	// �ł������ǂȂ񂩕ρA�ʂ̃Q�[���������ĉ��ϗ\�m����
	
	// TODO : ���[�U�[�̔��ˏ����̕ύX
	// ���̔��˂��Ɣ��˂Ƃ͌����Ȃ�
	// �G�t�F�N�g�����ɓ��������n�_�Ŏ~�߂鏈������肾���ǃG�t�F�N�g�̊g�嗦�����܂���������΂����邩�炪��΂�


	firePos_ = firePos;
	scale_ = model_scale;
	pos_ = *firePos;
	vec_ = vec;
	isEnabled_ = true;

	// �x�N�g�������̉�]�s����쐬
	rotMtx_ = MGetRotVec2(init_model_direction, *vec_);

	// �x�N�g�������̉�]�s�񂩂�I�C���[�p���o��
	MATRIX rotEffectMtx = MGetRotVec2(init_effect_direction, *vec_);
	bool isGimbalLock = false;
	VECTOR effectRot = MathUtil::ToEulerAngles(rotEffectMtx, isGimbalLock);

	auto& effectManager = Effekseer3DEffectManager::GetInstance();
	if (!isContinue)
	{
		// ���[�U�[�̃G�t�F�N�g�̍Đ�
		effectManager.PlayEffectFollow(laserEffectHandle_, EffectID::normal_laser, firePos_, effect_scale, 1.0f, effectRot);
	}
	else
	{
		// �p�����[�U�[�̃G�t�F�N�g�̍Đ�
		effectManager.PlayEffectFollow(laserEffectHandle_, EffectID::continue_laser, firePos_, effect_scale, 1.0f, effectRot);
	}

	// �����蔻��Ɏg�p���郂�f���̐ݒ�
	pModel_ = std::make_unique<Model>(modelHandle);	// �C���X�^���X����
	pModel_->SetUseCollision(true);					// �����蔻��ݒ�
	pModel_->SetScale(scale_);		// �g�嗦
	pModel_->SetRotMtx(rotMtx_);	// ��]�s��
	pModel_->SetPos(pos_);			// �ʒu
	pModel_->Update();				// �����蔻��̍X�V
}

NormalLazer::~NormalLazer()
{
	auto& effectManager = Effekseer3DEffectManager::GetInstance();
	effectManager.DeleteEffect(laserEffectHandle_);
}

void NormalLazer::Update()
{
	// ��ɔ��ˈʒu�ɒǏ]����
	pos_ = *firePos_;

	// �x�N�g�������̉�]�s������f���ɐݒ�
	rotMtx_ = MGetRotVec2(init_model_direction, *vec_);

	// �x�N�g�������̉�]�s�񂩂�I�C���[�p���o��
	MATRIX rotEffectMtx = MGetRotVec2(init_effect_direction, *vec_);
	bool isGimbalLock = false;
	VECTOR effectRot = MathUtil::ToEulerAngles(rotEffectMtx, isGimbalLock);

	// �G�t�F�N�g�̉�]���̐ݒ�
	auto& effectManager = Effekseer3DEffectManager::GetInstance();
	effectManager.SetEffectRot(laserEffectHandle_, effectRot);

	// �G�t�F�N�g�̊g�嗦�̐ݒ�
	effectManager.SetEffectScale(laserEffectHandle_, effect_scale);

	// �G�t�F�N�g�̃`���[�W���I�������甭��
	collisionAndEffectDifferenceTimer_.Update(1);
	if (collisionAndEffectDifferenceTimer_.IsTimeOut())
	{
		scale_.x = -1.0f;
	}

	// ���f���̐ݒ�
	pModel_->SetRotMtx(rotMtx_);
	pModel_->SetScale(scale_);
	pModel_->SetPos(pos_);
	pModel_->Update();
}

void NormalLazer::Draw()
{
#ifdef _DEBUG
	pModel_->Draw();
#endif
}

void NormalLazer::Stop(const VECTOR pos)
{
	VECTOR vec = VSub(pos, *firePos_);
	float size = VSize(vec);

	auto& effectManager = Effekseer3DEffectManager::GetInstance();
	effectManager.SetEffectScale(laserEffectHandle_, { effect_scale.x, effect_scale.y, size / 310.0f });

	scale_.x = 1.0f;
	pModel_->SetScale(scale_);
	pModel_->Update();

	//// ���[�U�[�̃G�t�F�N�g���X�g�b�v
	//auto& effectManager = Effekseer3DEffectManager::GetInstance();
	//effectManager.DeleteEffect(laserEffectHandle_);
//
//	isRefrect_ = true;
//	pos_ = pos;
//	effectPos_ = pos;
//	pModel_->SetPos(pos);
//
//#if true
//	// ���˃x�N�g���̍쐬
//	VECTOR inversionVec = VScale(*vec_, -1);
//	float dot = VDot(inversionVec, norm);
//	dot *= 2.0f;
//	VECTOR normVec = VScale(norm, dot);
//	actualVec_ = VAdd(*vec_, normVec);
//	actualVec_ = VScale(actualVec_, 48.0f);
//#else
//	vec_ = VScale(vec_, -1);
//#endif
//
//	// �x�N�g�������̉�]�s������f���ɐݒ�
//	MATRIX rotModelMtx = MGetRotVec2(init_model_direction, actualVec_);
//	MV1SetRotationMatrix(pModel_->GetModelHandle(), rotModelMtx);
//
//	// �x�N�g�������̉�]�s�񂩂�I�C���[�p���o��
//	MATRIX rotEffectMtx = MGetRotVec2(init_effect_direction, actualVec_);
//	bool isGimbalLock = false;
//	VECTOR effectRot = MathUtil::ToEulerAngles(rotEffectMtx, isGimbalLock);
//
//	// ���[�U�[�̃G�t�F�N�g�̍Đ�
//	effectManager.PlayEffectFollow(laserEffectHandle_, EffectID::refrect_laser, &effectPos_, effect_scale, 1.0f, effectRot);
//
//	pModel_->Update();
}

// ���[�U�[�̃G�t�F�N�g�̍Đ����I�����Ă����瓖���蔻��p�̃��f�����폜
void NormalLazer::ConfirmDelete()
{
	auto& effectManager = Effekseer3DEffectManager::GetInstance();
	if (!effectManager.IsPlayingEffect(laserEffectHandle_))
	{
		isEnabled_ = false;
	}
}

VECTOR NormalLazer::GetVec() const
{
	return *vec_;
}