#include "NormalEnemy.h"
#include "../Util/Model.h"
#include "../Laser/LazerManager.h"
#include "../Player.h"
#include "../common.h"
#include "../Vector2.h"

namespace
{
	// ���[�U�[�̔��ˈʒu�̃t���[��
	constexpr int lazer_fire_frame_pos = 37;

	// �����蔻��̔��a
	constexpr float collision_radius = 48.0f;

	constexpr int anim_frame = 1;

	constexpr VECTOR init_pos = { 60, 0, 800 };
	constexpr VECTOR model_scale = { 0.7f, 0.7f, 0.7f };
	constexpr VECTOR model_rot = { 0, 0, 0 };
}

NormalEnemy::NormalEnemy(int modelHandle, std::shared_ptr<Player> pPlayer, std::shared_ptr<LazerManager> pLazerManager)
{
	pPlayer_ = pPlayer;
	pLaserManager_ = pLazerManager;
	moveVec_.x = 10;
	pos_ = init_pos;
	rot_ = model_rot;
	utilTimerTable_["normalLaserFireInterval"] = (GetRand(10) + 1) * 60;
	collisionRadius_ = collision_radius;

	pModel_ = std::make_unique<Model>(modelHandle);
	pModel_->SetPos(pos_);
	pModel_->SetRot(rot_);
	pModel_->SetScale(model_scale);
	pModel_->ChangeAnimation(anim_frame, true, false, 8);
	pModel_->Update();
}

NormalEnemy::~NormalEnemy()
{
}

void NormalEnemy::Update()
{
	// ���[�U�[�̔��ˈʒu�̃t���[�����W�̎擾
	// �Ǐ]�����邽�߂ɖ��t���[���擾
	firePos_ = MV1GetFramePosition(pModel_->GetModelHandle(), lazer_fire_frame_pos);

	// �v���C���[�Ɍ������x�N�g�����쐬
	toTargetVec_ = VSub(pPlayer_->GetPos(), firePos_);
	toTargetVec_ = VNorm(toTargetVec_);

	utilTimerTable_["normalLaserFireInterval"].Update(1);
	if (utilTimerTable_["normalLaserFireInterval"].IsTimeOut())
	{
		// ���[�U�[�𔭎�
		pLaserManager_->Create(LaserType::NORMAL, &firePos_, &toTargetVec_, &moveVec_);
		utilTimerTable_["normalLaserFireInterval"].Reset();
	}

	// �T�C���J�[�u�ړ�
	// �����Ă���悤�Ɍ����邽��
	SinWave(50, 5);

	VECTOR screenPos = ConvWorldPosToScreenPos(pos_);
	if (screenPos.x > common::screen_width || screenPos.x < 0)
		moveVec_.x *= -1;

	pos_ = VAdd(pos_, moveVec_);

	pModel_->SetRot(rot_);
	pModel_->SetPos(pos_);
	pModel_->Update();
}

void NormalEnemy::Draw()
{
	pModel_->Draw();

#ifdef _DEBUG
//	DrawSphere3D(MV1GetFramePosition(pModel_->GetModelHandle(), lazer_fire_frame_pos), 10, 8, 0xff0000, 0xff0000, 0xff0000);
//	DrawSphere3D(pos_, collisionRadius_, 8, 0xff0000, 0xff0000, 0xff0000);
#endif
}