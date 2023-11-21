#include "Player.h"
#include "Shield.h"
#include "Util/InputState.h"
#include "Util/Model.h"
#include "Util//Effekseer3DEffectManager.h"
#include "Util/Debug.h"
#include "Util/Range.h"
#include "common.h"
#include <string>

namespace
{
	// �v���C���[���f���̃t�@�C���̃p�X
	const std::string model_file_hierarchy = "Data/Model/MV1/";
	const std::string model_file_name = "Player";
	const std::string model_file_extension = ".mv1";

	// �v���C���[�̈ړ���
	constexpr VECTOR player_vec_up = { 0, 1, 0 };
	constexpr VECTOR player_vec_down = { 0, -1, 0 };
	constexpr VECTOR player_vec_right = { 1, 0, 0 };
	constexpr VECTOR player_vec_left = { -1, 0, 0 };

	// �����ʒu
	constexpr VECTOR init_pos = { 0, 0, 0 };

	// ���f���̉�]��
	constexpr VECTOR init_rot = { 0, 0, 0 };

	// ���f���̊g�嗦
	constexpr VECTOR model_scale = { 0.5f, 0.5f, 0.5f };

	// �v���C���[�̒ʏ�ړ����x
	constexpr float move_normal_speed = 1.5f;

	// �G�l���M�[�Q�[�W
	constexpr float energy_gauge_total_amount = 1000.0f;

	// �G�l���M�[�̉񕜗�
	constexpr float energy_recovery_amount = 10.0f;

	// �v���C���[�̓����蔻��̔��a
	constexpr float model_collision_radius = 30.0f;

	// ���G���Ԃ̃t���[����
	constexpr int ultimate_frames = 120;

	// �c�@��
	constexpr int max_hp = 1000;

	// ���t���[���O�܂ňʒu����ۑ����邩
	constexpr int log_frame = 2;
}

//  �R���X�g���N�^
Player::Player() :
	moveVec_(VGet(0.0f, 0.0f, 0.0f)),
	hp_(max_hp),
	ultimateTimer_(0),
	isInput_(false),
	moveSpeed_(move_normal_speed),
	energyGauge_(energy_gauge_total_amount),
	playerDeadEffectHandle_(-1),
	isPlayGameOverEffect_(false),
	isReverseMoveVec_(false),
	boostEffectScale_(20.0f),
	boostEffectSpeed_(1.0f)
{
	pos_ = init_pos;
	rot_ = init_rot;

	// �v���C���[���f���̃C���X�^���X�̐���
	std::string filePath = model_file_hierarchy + model_file_name + model_file_extension;
	pModel_ = std::make_shared<Model>(filePath.c_str());

	// �V�[���h�̃C���X�^���X�̐���
	pShield_ = std::make_shared<Shield>(*this);

	// ���f���̊g�嗦�̐ݒ�
	pModel_->SetScale(model_scale);

	// ��]���̐ݒ�
	pModel_->SetRot(VGet(rot_.x, rot_.y, rot_.z));

	// �ʒu���̐ݒ�
	pModel_->SetPos(pos_);

	// �A�j���[�V�����Ɠ����蔻��̍X�V
	pModel_->Update();

	// �u�[�X�g�G�t�F�N�g�̍Đ�
	Effekseer3DEffectManager::GetInstance().PlayEffectLoopAndFollow(boostEffectHandle_, EffectID::player_boost, &pos_, boostEffectScale_, boostEffectSpeed_, { rot_.x, 0.0f, 0.0f });
}

//  �f�X�g���N�^
Player::~Player()
{
}

// �X�V
void Player::Update(float cameraYaw)
{
	auto& effectManager = Effekseer3DEffectManager::GetInstance();

	// ���X�e�B�b�N�̓��͏��̎擾
	int up = InputState::IsPadStick(PadLR::LEFT, PadStickInputType::UP);
	int down = InputState::IsPadStick(PadLR::LEFT, PadStickInputType::DOWN);
	int left = InputState::IsPadStick(PadLR::LEFT, PadStickInputType::LEFT);
	int right = InputState::IsPadStick(PadLR::LEFT, PadStickInputType::RIGHT);

	VECTOR moveUp = VTransform(player_vec_up, MGetRotY(cameraYaw));
	VECTOR moveDown = VTransform(player_vec_down, MGetRotY(cameraYaw));
	VECTOR moveRight = VTransform(player_vec_right, MGetRotY(cameraYaw));
	VECTOR moveLeft = VTransform(player_vec_left, MGetRotY(cameraYaw));

	// �G�l���M�[�̏���
	EnergyProcess();

	// �ړ����̏�����
	isInput_ = false;
	moveVec_ = VGet(0, 0, 0);
	VECTOR moveVecX = VGet(0, 0, 0);
	VECTOR moveVecY {0, 0, 0};

	// �X�e�B�b�N�����͂���Ă�����ړ��x�N�g���ɃX�e�B�b�N���X���Ă�������̃x�N�g������
	// �X�e�B�b�N�̌X�����킢�ɂ���ăx�N�g�����傫���Ȃ�
	if (up > 0)
	{
		moveVecY = VScale(moveUp, up);
		isInput_ = true;
	}
	if (left > 0)
	{
		moveVecX = VScale(moveLeft, left);
		isInput_ = true;
	}
	if (down > 0)
	{
		moveVecY = VScale(moveDown, down);
		isInput_ = true;
	}
	if (right > 0)
	{
		moveVecX = VScale(moveRight, right);
		isInput_ = true;
	}

	// �X�e�B�b�N�����͂���Ă���ꍇ�݈̂ړ�
	if (isInput_)
	{
		// �v���C���[���猩��x������z�����̃x�N�g���𑫂��Ĉړ��x�N�g�����쐬����
		moveVec_ = VAdd(moveVecY, moveVecX);

		// �v���C���[�̃X�s�[�h���|����
		moveVec_ = VScale(moveVec_, moveSpeed_);

		// �쐬�����ړ��x�N�g���ō��W�̈ړ�
		VECTOR tempPos = VAdd(pos_, moveVec_);

		// ���[���h���W���X�N���[�����W�ɕϊ�
		VECTOR screenPos = ConvWorldPosToScreenPos(tempPos);

		if (screenPos.x > common::screen_width)
		{
			screenPos.x = common::screen_width;
			VECTOR a = ConvScreenPosToWorldPos(screenPos);
			pos_.x = a.x;
			pos_.y = a.y;
		}
		else if (screenPos.x < 0)
		{
			screenPos.x = 0;
			VECTOR a = ConvScreenPosToWorldPos(screenPos);
			pos_.x = a.x;
			pos_.y = a.y;
		}
		else if (screenPos.y > common::screen_height)
		{
			screenPos.y = common::screen_height;
			VECTOR a = ConvScreenPosToWorldPos(screenPos);
			pos_.x = a.x;
			pos_.y = a.y;

		}
		else if (screenPos.y < 0)
		{
			screenPos.y = 0;
			VECTOR a = ConvScreenPosToWorldPos(screenPos);
			pos_.x = a.x;
			pos_.y = a.y;
		}
		else
		{
			pos_ = tempPos;
		}
	}

	// ���G���Ԃ̃^�C�}�[�̍X�V
	// 0�ȉ��ɂ͂Ȃ�Ȃ�
	ultimateTimer_ = (std::max)(--ultimateTimer_, 0);

	// �ړ��x�N�g���̑傫������v���C���[�̌X������Z�o
	// X����]�͐i��ł���悤�Ɍ�����悤�ɏ��30�x�𑫂�
	float rotX = 30.0f * DX_PI_F / 180.0f;
	rot_ = { rotX + moveVec_.z * 0.01f, 0.0f, -moveVec_.x * 0.01f };

	effectManager.SetEffectRot(boostEffectHandle_, { rot_.x + DX_PI_F, rot_.y, -rot_.z});
	effectManager.SetEffectScale(boostEffectHandle_, boostEffectScale_);
	effectManager.SetEffectSpeed(boostEffectHandle_, boostEffectSpeed_);

	// �ʒu���W�̐ݒ�
	pModel_->SetPos(pos_);

	// �����Ă�������̐ݒ�
	pModel_->SetRot(rot_);

	// �A�j���[�V������i�߂�
	pModel_->Update();

	// �V�[���h�̍X�V
	pShield_->Update();
}

// �Փˎ��̍X�V
bool Player::OnDamageUpdate()
{
	// �u�[�X�g���̃G�t�F�N�g�̍Đ��̃X�g�b�v
	Effekseer3DEffectManager::GetInstance().DeleteEffect(boostEffectHandle_);
	
	// �ړ��x�N�g���𔽓]���Ă��Ȃ������甽�]
	// ���ɔ��]���Ă����甽�]���Ȃ�
	if (!isReverseMoveVec_)
	{
		isReverseMoveVec_ = true;
		moveVec_ = VScale(moveVec_, -1.0f);
	}

	// �x�N�g�������X�ɏ���������
	moveVec_ = VScale(moveVec_, 0.96f);

	// �쐬�����ړ��x�N�g���ō��W�̈ړ�
	pos_ = VAdd(pos_, moveVec_);

	// �x�N�g��������̑傫�������������Ȃ�����Q�[���I�[�o�[�G�t�F�N�g�Đ�
	if(VSize(moveVec_) <= 1.0f)
	{
		// �_���[�W����
		OnDamage();

		// �G�t�F�N�g���Đ����I������true��Ԃ�
		if (!Effekseer3DEffectManager::GetInstance().IsPlayingEffect(playerDeadEffectHandle_) && isPlayGameOverEffect_)
		{
			// ������
			isReverseMoveVec_ = false;

			isPlayGameOverEffect_ = false;

			return true;
		}

		// �Q�[���I�[�o�[�G�t�F�N�g���Đ����ĂȂ�������Đ�
		// ���ɍĐ����Ă�����Đ����Ȃ�
		if (!isPlayGameOverEffect_)
		{
			isPlayGameOverEffect_ = true;
			Effekseer3DEffectManager::GetInstance().PlayEffect(playerDeadEffectHandle_, EffectID::player_dead, pos_, 50.0f, 0.5f);
		}
	}
	
	// �ʒu���W�̐ݒ�
	pModel_->SetPos(pos_);

	// �����Ă�������̐ݒ�
	pModel_->SetRot(VGet(VSize(moveVec_) / 5.0f, 0.0f, VSize(moveVec_) / 10.0f));

	// �A�j���[�V�����Ɠ����蔻��̍X�V
	pModel_->Update();

	// �������r���Ȃ̂�false��Ԃ�
	return false;
}

// �G�l���M�[����
void Player::EnergyProcess()
{
	energyGauge_ += energy_recovery_amount;

	// �G�l���M�[����ɒB������񕜂��~�߂�
	if (energyGauge_ > energy_gauge_total_amount)
	{
		energyGauge_ = energy_gauge_total_amount;
	}
}

// �`��
void Player::Draw()
{
	Debug::Log("playerPos", pos_);
	Debug::Log("energyGauge", energyGauge_);
	Debug::Log("�c�@", hp_);

	// ���G���Ԃ̓_��
	if (IsUltimate())
	{
		if ((ultimateTimer_ / 5) % 2 == 0)
		{
			return;
		}
	}

	// �v���C���[���f���̕`��
	if (!isPlayGameOverEffect_)
	{
		pModel_->Draw();
#ifdef _DEBUG
		DrawSphere3D(pos_, model_collision_radius, 8, 0xff0000, 0xff0000, false);
#endif 
	}
	pShield_->Draw();
}

// �v���C���[�̃��X�|�[������
void Player::Respawn(VECTOR restartPos)
{
	// Y���̒l��傫������
	// ���̂܂܂��Ɠ��ɂ߂荞�ނ���
	restartPos = VGet(restartPos.x, restartPos.y + 200.0f, restartPos.z);

	// ���X�|�[��
	pos_ = restartPos;
}

// �_���[�W����
void Player::OnDamage()
{
	// ���G���Ԓ��̓_���[�W���󂯂Ȃ�
	if (IsUltimate()) return;

	// HP�����炷
	hp_--;

	// ���G���Ԃ̐ݒ�
	ultimateTimer_ = ultimate_frames;
}

// �v���C���[�����G���Ԓ���
bool Player::IsUltimate() const
{
	return (ultimateTimer_ > 0) ? true : false;
}

bool Player::IsLive() const
{
	return (hp_ > 0 ) ? true : false;
}

// �ʒu���̎擾
VECTOR Player::GetPos() const
{
	return pos_;
}

// �v���C���[�̓����蔻��̔��a�̎擾
float Player::GetCollsionRadius() const
{
	return model_collision_radius;
}

// �v���C���[���f���̃n���h���̎擾
int Player::GetModelHandle() const
{
	return pModel_->GetModelHandle();
}

std::shared_ptr<Shield> Player::GetShield() const
{
	return pShield_;
}
