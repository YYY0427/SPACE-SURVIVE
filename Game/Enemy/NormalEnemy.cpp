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
	constexpr float collision_radius = 300.0f;

	constexpr int anim_frame = 1;
}

NormalEnemy::NormalEnemy(int modelHandle, std::shared_ptr<Player> pPlayer, std::shared_ptr<LazerManager> pLazerManager, UnityGameObject data)
{
	toTargetVec_ = {};
	pPlayer_ = pPlayer;
	pLazerManager_ = pLazerManager;
	moveVec_.x = 10;
	pos_ = data.pos;
	rot_ = { data.rot.x, data.rot.y, data.rot.z};
	lazerFireIntervalTimer_ = (GetRand(10) + 1) * 60;
	lazerSpeed_ = 210.0f;
	collisionRadius_ = collision_radius;

	pModel_ = std::make_unique<Model>(modelHandle);
	pModel_->SetPos(pos_);
	pModel_->SetRot(rot_);
	pModel_->SetScale(data.scale);
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
	VECTOR tempVec = VSub(pPlayer_->GetPos(), firePos_);
	toTargetVec_ = VNorm(tempVec);
	toTargetVec_ = VScale(toTargetVec_, lazerSpeed_);

	lazerFireIntervalTimer_.Update(1);
	if (lazerFireIntervalTimer_.IsTimeOut())
	{
		// ���[�U�[�𔭎�
		pLazerManager_->Create(LazerType::NORMAL, &firePos_, &toTargetVec_, &moveVec_);
		lazerFireIntervalTimer_.Reset();
	}

	// �T�C���J�[�u�ړ�
	// �����Ă���悤�Ɍ����邽��
	SinWave(100, 10);

	VECTOR screenPos = ConvWorldPosToScreenPos(pos_);
	if (screenPos.x > common::screen_width || screenPos.x < 0)
		moveVec_.x *= -1;

	moveVec_.z = pPlayer_->GetMoveVecZ().z;
	pos_ = VAdd(pos_, moveVec_);

	pModel_->SetRot(rot_);
	pModel_->SetPos(pos_);
	pModel_->Update();
}

void NormalEnemy::Draw()
{
	pModel_->Draw();

#ifdef _DEBUG
//	DrawSphere3D(MV1GetFramePosition(pModel_->GetModelHandle(), lazer_fire_frame_pos), 100.0f, 8, 0xff0000, 0xff0000, 0xff0000);
//	DrawSphere3D(pos_, collisionRadius_, 8, 0xff0000, 0xff0000, 0xff0000);
#endif
}