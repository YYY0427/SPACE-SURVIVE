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
#include "../Laser/LaserManager.h"
#include "../Background.h"
#include "../Shield.h"
#include "../Laser/Laser.h"
#include "../common.h"
#include "../Vector2.h"
#include "../Triangle.h"
#include "../Flash.h"
#include "../ScreenEffect.h"
#include "../UIManager.h"

namespace
{
	// �Q�[���N���A���ɉ��t���[���ԉ�ʂ�h�炷��
	constexpr int game_clear_shake_frame = 60;
}

// �R���X�g���N�^
GameMainScene::GameMainScene(SceneManager& manager) :
	SceneBase(manager)
{
	screenHandle_ = MakeScreen(common::screen_width, common::screen_height);
	assert(screenHandle_ != -1);

	// �C���X�^���X����
	pUIManager_ = std::make_shared<UIManager>();
	pScreenEffect_ = std::make_shared<ScreenEffect>();
	pBackground_ = std::make_shared<Background>();
	pLaserManager_ = std::make_shared<LaserManager>();
	pPlayer_ = std::make_shared<Player>();
	pEnemyManager_ = std::make_shared<EnemyManager>(pPlayer_, pLaserManager_, pScreenEffect_, pUIManager_);
	pCamera_ = std::make_shared<Camera>();

	// �X�e�[�g�̏�����
	stateMachine_.AddState(
		State::NORMAL,
		[this]() { EntarNormalState(); },
		[this]() { UpdateNormalState(); },
		[this]() { ExitNormalState(); }
		);
	stateMachine_.AddState(
		State::GAME_CLEAR,
		[this]() { EntarGameClearState(); },
		[this]() { UpdateGameClearState(); },
		[this]() { ExitGameClearState(); }
		);
	stateMachine_.AddState(
		State::GAME_OVER,
		[this]() { EntarGameOverState(); },
		[this]() { UpdateGameOverState(); },
		[this]() { ExitGameOverState(); }
		);
	stateMachine_.SetState(State::NORMAL);
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

	// �^�C�}�[�̍X�V
	gameTimer_.Update(1);

	// �X�e�[�g�}�V���̍X�V
	stateMachine_.Update();

	// ��ʃG�t�F�N�g�̍X�V
	pScreenEffect_->Update();

	// UI�̍폜�m�F
	pUIManager_->DeleteCheck();

	// �t�F�[�h�̍X�V
	UpdateFade();
}

// �`��
void GameMainScene::Draw()
{
	// ��ʂ̕`����ύX
	SetDrawScreen(screenHandle_);

	// SetDrawScreen���g�p�����ꍇ�A�J�����������������̂ōēx�X�V
	pCamera_->Update();

	// ��ʂ̃N���A
	ClearDrawScreen();

	// �e�N���X�̕`��
	pBackground_->Draw();
	pEnemyManager_->Draw();
	pPlayer_->Draw();
	pLaserManager_->Draw();

	// �e�N���X��UI�̕`��
	pPlayer_->DrawUI();
	pEnemyManager_->DrawUI();

	// ���݂̃V�[���̃e�L�X�g�\��
	Debug::Log("GameMainScene");

	// �t�F�[�h�̕`��
	DrawFade(true);

	// ���U�C�N�t�F�[�h�̕`��
	DrawGaussFade(true);

	// ��ʂ̕`����߂�
	SetDrawScreen(DX_SCREEN_BACK);

	// SetDrawScreen���g�p�����ꍇ�A�J�����������������̂ōēx�X�V
	pCamera_->Update();

	// ��ʂ̕`��
	pScreenEffect_->Draw(screenHandle_);
}

void GameMainScene::EntarNormalState()
{
}

void GameMainScene::EntarGameClearState()
{
	waitTimer_ = 120;
}

void GameMainScene::EntarGameOverState()
{
}

void GameMainScene::UpdateNormalState()
{
	// �X�V
	pPlayer_->Update(pCamera_->GetCameraYaw());
	pEnemyManager_->Update(gameTimer_.GetTime());
	pLaserManager_->Update();
	pCamera_->Update();

	// �����蔻��
	Collision();

	// �|�[�Y��ʂɑJ��
	if (InputState::IsTriggered(InputType::PAUSE))
	{
		// �t�F�[�h�A�E�g�J�n
		StartFadeOut(200, 64);
		item_ = SceneItem::PAUSE;
	}

	// �Q�[���N���A
	if (pEnemyManager_->IsBossDied())
	{
		stateMachine_.SetState(State::GAME_CLEAR);
	}

	// �Q�[���I�[�o�[
	if (!pPlayer_->IsLive())
	{
		stateMachine_.SetState(State::GAME_OVER);
	}
}

void GameMainScene::UpdateGameClearState()
{
	waitTimer_.Update(1);
	if (waitTimer_.IsTimeOut())
	{
		// �Q�[���N���A���o
		pCamera_->GameClearUpdate(pPlayer_->GetPos());
		
		// TODO : �Q�[���N���A�̕������o�����o��ǉ�����

		// �^�C�g���ɖ߂�
		if (InputState::IsTriggered(InputType::DECISION))
		{
			// �t�F�[�h�A�E�g�J�n
			StartFadeOut(255, 5);
			item_ = SceneItem::TITLE;
		}	
	}
}

void GameMainScene::UpdateGameOverState()
{
	pLaserManager_->GraduallyAlphaDeleteAllLaser();
	pPlayer_->GameOverUpdate();
	pEnemyManager_->GameOverUpdate();
	pCamera_->GameOverUpdate(pPlayer_->GetPos());


	// �^�C�g���ɖ߂�
	if (InputState::IsTriggered(InputType::DECISION))
	{
		// �t�F�[�h�A�E�g�J�n
		StartFadeOut(255, 5);
		item_ = SceneItem::TITLE;
	}
}

void GameMainScene::ExitNormalState()
{
}

void GameMainScene::ExitGameClearState()
{
}

void GameMainScene::ExitGameOverState()
{
}

void GameMainScene::Collision()
{
	if (pPlayer_->GetShield()->IsShield())
	{
		// ���[�U�[�ƃV�[���h�̓����蔻��
		for (auto& laser : pLaserManager_->GetLazeres())
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
				auto pLaser = std::dynamic_pointer_cast<Laser>(laser.pLaser);
				pLaser->Stop(pPlayer_->GetShield()->GetPos());

				// ���˃��[�U�[���쐬(���ɍ쐬����Ă����ꍇ�X�V���s��)
				// �V�[���h�̖@�����
				VECTOR shieldNorm = pPlayer_->GetShield()->GetVertex()[0].norm;
				pLaserManager_->Reflect(pPlayer_->GetShield()->GetPos(), laser.pLaser->GetVec(), shieldNorm);
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
		pLaserManager_->DeleteReflectLaser();
	}

	// ���[�U�[�ƓG�̓����蔻��
	for (auto& laser : pLaserManager_->GetLazeres())
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
	for (auto& laser : pLaserManager_->GetLazeres())
	{
		if (laser.type == LaserType::REFLECT) continue;

		// ���[�U�[�ƃv���C���[�̓����蔻��
		MV1_COLL_RESULT_POLY_DIM result = MV1CollCheck_Sphere(laser.pLaser->GetModelHandle(), -1, pPlayer_->GetPos(), pPlayer_->GetCollsionRadius());

		// 1�ł��|���S���Ɠ������Ă�����
		if (result.HitNum > 0)
		{
			pPlayer_->OnDamage();

			// ��ʂ�h�炷
			pScreenEffect_->SetShake(3, 3, 10);

			pScreenEffect_->SetScreenColor(0xff0000, 50, 3);

			Debug::Log("������������������������������������������");

			// �����蔻����̌�n��
			MV1CollResultPolyDimTerminate(result);
			return;
		}

		// �����蔻����̌�n��
		MV1CollResultPolyDimTerminate(result);
	}
}

//void GameMainScene::CollisionRockUpdate()
//{
//	// �t�F�[�h�A�E�g���I��莟��V�[���J��
//	if (IsStartFadeOutAfterFadingOut())
//	{
//		if (pPlayer_->IsLive())
//		{
//			StartFadeIn();
//
//			// Update��ʏ펞��Update�ɕύX
//			updateFunc_ = &GameMainScene::NormalUpdate;
//			return;
//		}
//	}
//
//	if (!IsFadeing())
//	{
//		bool isEnd = pPlayer_->OnDamageUpdate();
//
//		if (isEnd)
//		{
//			// �t�F�[�h�A�E�g�̊J�n
//			StartFadeOut(255);
//		}
//	}
//
//	// �X�V
//	pCamera_->Update();
//	pLaserManager_->Update();
//	pEnemyManager_->Update(timer_.GetTime());
//
//	// �t�F�[�h�̍X�V
//	UpdateFade();
//}