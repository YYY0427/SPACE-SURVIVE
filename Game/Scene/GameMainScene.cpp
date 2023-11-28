#include "GameMainScene.h"
#include "SceneManager.h"
#include "TitleScene.h"
#include "DebugScene.h"
#include "PauseScene.h"
#include "../Util/Effekseer3DEffectManager.h"
#include "../Util/SoundManager.h"
#include "../Util/InputState.h"
#include "../Util/Debug.h"
#include "../Camera.h"
#include "../Player.h"
#include "../Planet/PlanetManager.h"
#include "../Planet/PlanetBase.h"
#include "../Enemy/EnemyManager.h"
#include "../Laser/LazerManager.h"
#include "../Background.h"
#include "../Shield.h"
#include "../common.h"

namespace
{
}

// �R���X�g���N�^
GameMainScene::GameMainScene(SceneManager& manager) :
	SceneBase(manager),
	updateFunc_(&GameMainScene::NormalUpdate)
{
	// �C���X�^���X����
	pBackground_ = std::make_shared<Background>();
	pLazerManager_ = std::make_shared<LazerManager>();
	pPlayer_ = std::make_shared<Player>();
	pEnemyManager_ = std::make_shared<EnemyManager>(pPlayer_, pLazerManager_);
	pPlanetManager_ = std::make_shared<PlanetManager>();
	pCamera_ = std::make_shared<Camera>();
}

// �f�X�g���N�^
GameMainScene::~GameMainScene()
{
	// �S�ẴG�t�F�N�g�̍폜
	Effekseer3DEffectManager::GetInstance().DeleteAllEffect();
}

// �����o�֐��|�C���^�̍X�V
void GameMainScene::Update()
{
	timer_.Update(1);
	
	(this->*updateFunc_)();
}

// �`��
void GameMainScene::Draw()
{
	LONGLONG start = GetNowHiPerformanceCount();

	// �e�N���X�̕`��
	pBackground_->Draw();
	pPlanetManager_->Draw();
	pEnemyManager_->Draw();
	pPlayer_->Draw();
	pLazerManager_->Draw();

	// �e�N���X��UI�̕`��
	pPlayer_->DrawUI();
	pEnemyManager_->DrawUI();

	drawTime_ = GetNowHiPerformanceCount() - start;

	float rate = static_cast<float>(updateTime_ + drawTime_) / 16666.6f;
	int width = static_cast<int>(common::screen_width * rate);
	DrawBox(0, common::screen_height - 16, width, common::screen_height, 0xff0000, true);

	rate = static_cast<float>(updateTime_) / 16666.6f;
	width = static_cast<int>(common::screen_width * rate);
	DrawBox(0, common::screen_height - 16, width, common::screen_height, 0x0000ff, true);

	// ���݂̃V�[���̃e�L�X�g�\��
	Debug::Log("GameMainScene");

	// �t�F�[�h�̕`��
	DrawFade(true);

	// ���U�C�N�t�F�[�h�̕`��
	DrawGaussFade(true);
}

// �ʏ�̍X�V
void GameMainScene::NormalUpdate()
{
	// �t�F�[�h���I��莟��V�[���J��
	if (IsStartFadeOutAfterFadingOut())
	{
		switch (item_)
		{
			// �|�[�Y�V�[���ɑJ��
		case SceneItem::PAUSE:
			manager_.PushScene(new PauseScene(manager_));
			break;

		case SceneItem::TITLE:
#ifdef _DEBUG
			manager_.ChangeScene(new DebugScene(manager_));
#else
			manager_.ChangeScene(new TitleScene(manager_));
#endif
			return;
		}
		// PushScene����̂ŃV�[�����c�邽�߃t�F�[�h�C���̐ݒ�
		StartFadeIn();
		return;
	}

	LONGLONG start = GetNowHiPerformanceCount();

	// �X�V
	pPlayer_->Update(pCamera_->GetCameraYaw());
	pEnemyManager_->Update(timer_.GetTime());
	pLazerManager_->Update();
	pPlanetManager_->Update();
	pCamera_->Update();

	if (pPlayer_->GetShield()->GetIsShield())
	{
		// ���[�U�[�ƃV�[���h�̓����蔻��
		for (auto& laser : pLazerManager_->GetLazeres())
		{
			// �L���[�u���[�U�[�͔��˂ł��Ȃ��̂Ŕ�΂�
			if (laser.type == LaserType::CUBE) continue;
			if (laser.type == LaserType::REFLECT) continue;

			VECTOR leftTop = pPlayer_->GetShield()->GetVertex()[0].pos;
			VECTOR rightTop = pPlayer_->GetShield()->GetVertex()[1].pos;
			VECTOR leftBottom = pPlayer_->GetShield()->GetVertex()[2].pos;
			VECTOR rightBottom = pPlayer_->GetShield()->GetVertex()[3].pos;

			// �V�[���h�͓�̃|���S������ł��Ă�̂œ�̃|���S���Ƃ��`�F�b�N
			MV1_COLL_RESULT_POLY_DIM result = MV1CollCheck_Triangle(laser.pLaser->GetModelHandle(), -1, leftTop, rightTop, leftBottom);
			MV1_COLL_RESULT_POLY_DIM result2 = MV1CollCheck_Triangle(laser.pLaser->GetModelHandle(), -1, rightBottom, leftBottom, rightTop);

			// 1�ł��|���S���Ɠ������Ă�����
			if (result.HitNum > 0 || result2.HitNum > 0)
			{
				laser.pLaser->SetIsReflect(true);

				// ���[�U�[���~�߂�
				laser.pLaser->Stop(pPlayer_->GetShield()->GetPos());

				// ���˃��[�U�[���쐬(���ɍ쐬����Ă����ꍇ�X�V���s��)
				// �V�[���h�̖@�����
				VECTOR shieldNorm = pPlayer_->GetShield()->GetVertex()[0].norm;
				pLazerManager_->Reflect(pPlayer_->GetShield()->GetPos(), laser.pLaser->GetVec(), shieldNorm);
			}
			else
			{
				laser.pLaser->SetIsReflect(false);
			}

			// �����蔻����̌�n��
			MV1CollResultPolyDimTerminate(result);
			MV1CollResultPolyDimTerminate(result2);
		}
	}
	else
	{
		pLazerManager_->DeleteReflectLaser();
	}

	// ���[�U�[�ƓG�̓����蔻��
	for (auto& laser : pLazerManager_->GetLazeres())
	{
		// ���ˉ\�ȃ��[�U�[�ȊO�̏ꍇ�͔�����s��Ȃ�
		if (laser.type != LaserType::REFLECT) continue;

		// �S�Ă̓G���`�F�b�N
		for (auto& enemy : pEnemyManager_->GetEnemies())
		{
			// ���[�U�[�̃��f���ƓG�����̂ɂ݂��Ăē����蔻��
			MV1_COLL_RESULT_POLY_DIM result = MV1CollCheck_Sphere(laser.pLaser->GetModelHandle(), -1, enemy->GetPos(), enemy->GetCollisionRadius());

			// �|���S����ɂł��������Ă�����
			if (result.HitNum > 0)
			{
				enemy->OnDamage(1.0f, result.Dim->HitPosition);
			}

			// �����蔻����̌�n��
			MV1CollResultPolyDimTerminate(result);
		}
	}

	// ���[�U�[�ƃv���C���[�̓����蔻��
	for (auto& laser : pLazerManager_->GetLazeres())
	{
		// ���G���Ԓ��Ȃ瓖����Ȃ�
		if (pPlayer_->IsUltimate()) continue;

		if (laser.type == LaserType::REFLECT) continue;

		// ���[�U�[�ƃv���C���[�̓����蔻��
		MV1_COLL_RESULT_POLY_DIM result = MV1CollCheck_Sphere(laser.pLaser->GetModelHandle(), -1, pPlayer_->GetPos(), pPlayer_->GetCollsionRadius());

		// 1�ł��|���S���Ɠ������Ă�����
		if (result.HitNum > 0)
		{
			// Update���Q�[���I�[�o�[����Update�ɕύX
			updateFunc_ = &GameMainScene::CollisionRockUpdate;

			// �����蔻����̌�n��
			MV1CollResultPolyDimTerminate(result);
			return;
		}

		// �����蔻����̌�n��
		MV1CollResultPolyDimTerminate(result);
	}

	updateTime_ = GetNowHiPerformanceCount() - start;

	// �|�[�Y��ʂɑJ��
	if (InputState::IsTriggered(InputType::PAUSE))
	{
		// �t�F�[�h�A�E�g�J�n
		StartFadeOut(200, 64);
		item_ = SceneItem::PAUSE;
	}

	// �Q�[���I�[�o�[
	if (!pPlayer_->IsLive())
	{
		// �t�F�[�h�A�E�g�J�n
		StartFadeOut(255, 10);
		item_ = SceneItem::TITLE;
	}

	// �t�F�[�h�̍X�V
	UpdateFade();
}

void GameMainScene::CollisionRockUpdate()
{
	// �t�F�[�h�A�E�g���I��莟��V�[���J��
	if (IsStartFadeOutAfterFadingOut())
	{
		if (pPlayer_->IsLive())
		{
			StartFadeIn();

			// Update��ʏ펞��Update�ɕύX
			updateFunc_ = &GameMainScene::NormalUpdate;
			return;
		}
	}

	if (!IsFadeing())
	{
		bool isEnd = pPlayer_->OnDamageUpdate();

		if (isEnd)
		{
			// �t�F�[�h�A�E�g�̊J�n
			StartFadeOut(255);
		}
	}

	// �X�V
	pCamera_->Update();
	pLazerManager_->Update();
	pEnemyManager_->Update(timer_.GetTime());

	// �t�F�[�h�̍X�V
	UpdateFade();
}