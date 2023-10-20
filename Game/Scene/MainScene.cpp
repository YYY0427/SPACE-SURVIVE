#include "MainScene.h"
#include "TitleScene.h"
#include "PauseScene.h"
#include "SceneManager.h"
#include "../common.h"
#include "../Util/InputState.h"
#include "../Util/Effekseer3DEffectManager.h"
#include "../Util/SoundManager.h"
#include "../Util/Debug.h"
#include "../Camera.h"
#include "../Player.h"
#include "../SkyDome.h"
#include "../Rock/RockManager.h"
#include "../Rock/RockBase.h"
#include <DxLib.h>

// �R���X�g���N�^
MainScene::MainScene(SceneManager& manager) :
	SceneBase(manager),
	updateFunc_(&MainScene::NormalUpdate)
{
//	// �I�u�W�F�N�g�̔z�u�f�[�^�̓ǂݍ���
//	pDataReader_ = std::make_shared<DataReaderFromUnity>();
//	pDataReader_->LoadUnityGameObjectData();
//
//	pPlayer_ = std::make_shared<Player>(pDataReader_->GetPlayerData());
//	pRockManager_ = std::make_shared<RockManager>(pDataReader_->GetRockData(), pPlayer_);
//	pCamera_ = std::make_shared<Camera>(pPlayer_);
////	pSkyDome_ = std::make_shared<SkyDome>(pPlayer_);
//
//	// �R���X�g���N�^�œn���Ȃ��|�C���^�̐ݒ�
//	pPlayer_->SetCameraPointer(pCamera_);
}

// �f�X�g���N�^
MainScene::~MainScene()
{
	// �����Ȃ�
}

// �����o�֐��|�C���^�̍X�V
void MainScene::Update()
{
	(this->*updateFunc_)();
}

// �ʏ�̍X�V
void MainScene::NormalUpdate()
{
//	// �e�N���X�̍X�V
////	pSkyDome_->Update();
//	pCamera_->Update();
//	pPlayer_->Update();
//	pRockManager_->Update();
//
//	// �G�ƂԂ�������Q�[���I�[�o�[
//	for (auto& rock : pRockManager_->GetRocks())
//	{
//		MV1_COLL_RESULT_POLY_DIM result = MV1CollCheck_Sphere(rock->GetModelHandle(), -1, pPlayer_->GetPos(), pPlayer_->GetCollsionRadius());
//		if (result.HitNum > 0)
//		{
//			// Update���Q�[���I�[�o�[����Update�ɕύX
//			updateFunc_ = &MainScene::GameOverUpdate;
//		}
//	}

	// �|�[�Y��ʂɑJ��
	if (InputState::IsTriggered(InputType::PAUSE))
	{
		// �t�F�[�h�A�E�g�J�n
		StartFadeOut(200, 64);
	}

	// �t�F�[�h���I��莟��V�[���J��
	if (IsStartFadeOutAfterFadingOut())
	{
		// �G�t�F�N�g��S�Ď~�߂�
		Effekseer3DEffectManager::GetInstance().StopAllEffect();

		// PushScene����̂ŃV�[�����c�邽�߃t�F�[�h�C���̐ݒ�
		StartFadeIn();

		// �V�[���J��
		manager_.PushScene(new PauseScene(manager_));
		return;
	}

	// �t�F�[�h�̍X�V
	UpdateFade();
}

void MainScene::GameOverUpdate()
{
	// �t�F�[�h�A�E�g���I��莟��V�[���J��
	if (IsStartFadeOutAfterFadingOut())
	{
		manager_.ChangeScene(new TitleScene(manager_));
		return;
	}

	// �J�����̍X�V
//	pCamera_->Update();

	// �v���C���[�̃Q�[���I�[�o�[���̍X�V���I�����t�F�[�h���ĂȂ�������t�F�[�h�A�E�g�J�n
	if (pPlayer_->CollisionRockUpdate() && !IsFadeing())
	{
		// �t�F�[�h�A�E�g�̊J�n
		StartFadeOut(255);
	}
	// �t�F�[�h�̍X�V
	UpdateFade();
}

// �`��
void MainScene::Draw()
{
	// ���݂̃V�[���̃e�L�X�g�\��
	Debug::Log("MainScene");

	// �e�N���X�̕`��
//	pSkyDome_->Draw();
//	pRockManager_->Draw();
//	pPlayer_->Draw();

	// �t�F�[�h�̕`��
	DrawFade(true);

	// ���U�C�N�t�F�[�h�̕`��
	DrawGaussFade(true);
}