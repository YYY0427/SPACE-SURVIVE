#include "NormalEnemy.h"
#include "../Util/Model.h"
#include "../Laser/LazerManager.h"
#include "../Player.h"

namespace
{
	constexpr int lazer_fire_frame_pos = 1374;
	constexpr float collision_radius = 2000.0f;
}

NormalEnemy::NormalEnemy(int modelHandle, std::shared_ptr<Player> pPlayer, std::shared_ptr<LazerManager> pLazerManager, UnityGameObject data)
{
	pPlayer_ = pPlayer;
	pLazerManager_ = pLazerManager;
	pModel_ = std::make_unique<Model>(modelHandle);
	pos_ = data.pos;
	rot_ = { data.rot.x, data.rot.y + 180.0f * DX_PI_F / 180.0f , data.rot.z};
	pModel_->SetScale(data.scale);
	lazerFireIntervalTimer_ = 60 * 5;
	lazerSpeed_ = 210.0f;
	collisionRadius_ = collision_radius;
}

NormalEnemy::~NormalEnemy()
{
}

void NormalEnemy::Update()
{
	lazerFireIntervalTimer_.Update(1);
	if (lazerFireIntervalTimer_.IsTimeOut())
	{
		// ���[�U�[�̔��ˈʒu�̃t���[�����W�̎擾
		VECTOR firePos = MV1GetFramePosition(pModel_->GetModelHandle(), lazer_fire_frame_pos);

		// �v���C���[�Ɍ������x�N�g�����쐬
		VECTOR tempVec = VSub(pPlayer_->GetPos(), firePos);
		VECTOR toPlayerVec = VNorm(tempVec);
		toPlayerVec = VScale(toPlayerVec, lazerSpeed_);

		pLazerManager_->Create(LazerType::NORMAL, firePos, toPlayerVec);
		lazerFireIntervalTimer_.Reset();
	}

	pos_.z += pPlayer_->GetMoveVec().z;

	// �T�C���J�[�u�ړ�
	// �����Ă���悤�Ɍ����邽��
	SinWave(100, 10);

	pModel_->SetRot(rot_);
	pModel_->SetPos(pos_);
	pModel_->Update();
}

void NormalEnemy::Draw()
{
	pModel_->Draw();

#ifdef _DEBUG
	DrawSphere3D(MV1GetFramePosition(pModel_->GetModelHandle(), lazer_fire_frame_pos), 100.0f, 8, 0xff0000, 0xff0000, 0xff0000);
	DrawSphere3D(pos_, collisionRadius_, 8, 0xff0000, 0xff0000, 0xff0000);
#endif
}
