#include "Laser.h"
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

	// �G�t�F�N�g�̑��Đ����ԃt���[��
	constexpr int effect_total_play_frame = 100;

	// �G�t�F�N�g�̍Đ����x
	constexpr float effect_play_speed = 1.5f;
}

Laser::Laser(int modelHandle, VECTOR* firePos, VECTOR* vec, float fireFrameTime, bool isInfinity, int chargeFrame) :
	effectPos_({}),
	isReflect_(false)
{
	// TODO : ���[�U�[���v���C���[��Ǐ]����悤�ɂ���
	// �ł������ǂȂ񂩕ρA�ʂ̃Q�[���������ĉ��ϗ\�m����
	
	// TODO : ���[�U�[�̔��ˏ����̕ύX
	// �ł������ǂ�������������⊮�����

	// ������
	firePos_ = firePos;
	pos_ = *firePos;
	vec_ = vec;
	isInfinity_ = isInfinity;
	fireFrameTimer_ = fireFrameTime;
	scale_ = model_scale;
	isEnabled_ = true;

	// �x�N�g�������̉�]�s����쐬
	rotMtx_ = MGetRotVec2(init_model_direction, *vec_);

	// �x�N�g�������̉�]�s�񂩂�I�C���[�p���o��
	MATRIX rotEffectMtx = MGetRotVec2(init_effect_direction, *vec_);
	bool isGimbalLock = false;
	VECTOR effectRot = MathUtil::ToEulerAngles(rotEffectMtx, isGimbalLock);

	// �G�t�F�N�g�̃`���[�W���ԃt���[���ƃG�t�F�N�g�̍Đ����x����`���[�W���Ԃ����t���[���Ȃ̂����߂�
	chargeEffectTimer_ = effect_charge_frame / effect_play_speed;

	// �������[�U�[�̃G�t�F�N�g�̍Đ�
	auto& effectManager = Effekseer3DEffectManager::GetInstance();
	effectManager.PlayEffectFollow(laserEffectHandle_, EffectID::infinity_laser, firePos_, effect_scale, effect_play_speed, effectRot);

	// �����蔻��Ɏg�p���郂�f���̐ݒ�
	pModel_ = std::make_unique<Model>(modelHandle);	// �C���X�^���X����
	pModel_->SetUseCollision(true);					// �����蔻��ݒ�
	pModel_->SetScale(scale_);		// �g�嗦
	pModel_->SetRotMtx(rotMtx_);	// ��]�s��
	pModel_->SetPos(pos_);			// �ʒu
	pModel_->Update();				// �����蔻��̍X�V
}

Laser::~Laser()
{
	auto& effectManager = Effekseer3DEffectManager::GetInstance();
	effectManager.DeleteEffect(laserEffectHandle_);
}

void Laser::Update()
{
	// ��ɔ��ˈʒu�ɒǏ]����
	pos_ = *firePos_;

	// �x�N�g�������̉�]�s������f���ɐݒ�
	rotMtx_ = MGetRotVec2(init_model_direction, *vec_);

	// �x�N�g�������̉�]�s�񂩂�I�C���[�p���o��
	MATRIX rotEffectMtx = MGetRotVec2(init_effect_direction, *vec_);
	bool isGimbalLock = false;
	VECTOR effectRot = MathUtil::ToEulerAngles(rotEffectMtx, isGimbalLock);

	// �G�t�F�N�g�̃`���[�W���I�������甭��
	chargeEffectTimer_.Update(1);
	if (chargeEffectTimer_.IsTimeOut())
	{
		scale_.x = -1.0f;

		// �ʏ�̃��[�U�[�̏ꍇ
		if (!isInfinity_)
		{
			fireFrameTimer_.Update(1);
			if (fireFrameTimer_.IsTimeOut())
			{
				isEnabled_ = false;
			}
		}
	}

	// �G�t�F�N�g�̐ݒ�
	auto& effectManager = Effekseer3DEffectManager::GetInstance();
	effectManager.SetEffectRot(laserEffectHandle_, effectRot);			// ��]���̐ݒ�
	effectManager.SetEffectScale(laserEffectHandle_, effect_scale);		// �g�嗦�̐ݒ�

	// ���f���̐ݒ�
	pModel_->SetRotMtx(rotMtx_);
	pModel_->SetScale(scale_);
	pModel_->SetPos(pos_);
	pModel_->Update();
}

void Laser::Draw()
{
#ifdef _DEBUG
	pModel_->Draw();
#endif
}

void Laser::Stop(const VECTOR pos)
{
	VECTOR vec = VSub(pos, *firePos_);
	float size = VSize(vec);

	auto& effectManager = Effekseer3DEffectManager::GetInstance();
	effectManager.SetEffectScale(laserEffectHandle_, { effect_scale.x, effect_scale.y, size / 310.0f });

	scale_.x = 1.0f;
	pModel_->SetScale(scale_);
	pModel_->Update();
}

VECTOR Laser::GetVec() const
{
	return *vec_;
}