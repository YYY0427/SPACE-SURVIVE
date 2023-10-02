#include "MainScene.h"
#include "TitleScene.h"
#include "PauseScene.h"
#include "SceneManager.h"
#include "../common.h"
#include "../Util/InputState.h"
#include "../Util/Effekseer3DEffectManager.h"
#include "../Util/SoundManager.h"
#include "../Camera.h"
#include "../Player.h"
#include "../SkyDome.h"
#include "../EnemyManager.h"
#include "../Enemy.h"
#include <DxLib.h>

// �R���X�g���N�^
MainScene::MainScene(SceneManager& manager) :
	Scene(manager)
{
}

// �f�X�g���N�^
MainScene::~MainScene()
{
	// �����Ȃ�
}

// ������
void MainScene::Init()
{
	// �I�u�W�F�N�g�̔z�u�f�[�^�̓ǂݍ���
	pDataReader_ = std::make_shared<DataReaderFromUnity>();
	pDataReader_->LoadUnityGameObjectData();

	pPlayer_ = std::make_shared<Player>(pDataReader_->GetPlayerData());
	pCamera_ = std::make_shared<Camera>(pPlayer_);
	pEnemyManager_ = std::make_shared<EnemyManager>(pDataReader_->GetRockData(), pPlayer_);
	//	pSkyDome_ = std::make_shared<SkyDome>(pPlayer_);

	// �R���X�g���N�^�œn���Ȃ��|�C���^�̐ݒ�
	pPlayer_->SetCameraPointer(pCamera_);

	pPlayer_->Init();
	pEnemyManager_->Init();
	updateFunc_ = &MainScene::NormalUpdate;
}

// �����o�֐��|�C���^�̍X�V
void MainScene::Update()
{
	(this->*updateFunc_)();
}

// �ʏ�̍X�V
void MainScene::NormalUpdate()
{
	// �e�N���X�̍X�V
//	pSkyDome_->Update();
	pCamera_->Update();
	pPlayer_->Update();
	pEnemyManager_->Update();

	// �G�ƂԂ�������Q�[���I�[�o�[
	for (auto& enemies : pEnemyManager_->GetEnemies())
	{
		MV1_COLL_RESULT_POLY_DIM result = MV1CollCheck_Sphere(enemies->GetModelHandle(), -1, pPlayer_->GetPos(), pPlayer_->GetCollsionRadius());
		if (result.HitNum > 0)
		{
			// Update���Q�[���I�[�o�[����Update�ɕύX
			updateFunc_ = &MainScene::GameOverUpdate;
		}
	}

	// �|�[�Y��ʂɑJ��
	if (InputState::IsTriggered(InputType::PAUSE))
	{
		// �t�F�[�h�A�E�g�J�n
		StartFadeOut(200, 64);
	}
	// �t�F�[�h���I��莟��V�[���J��
	if (IsStartFadeOutAfterFadingOut())
	{
		Effekseer3DEffectManager::GetInstance().StopAllEffect();
		StartFadeIn();
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
	pCamera_->Update();

	// �v���C���[�̃Q�[���I�[�o�[���̍X�V���I�����t�F�[�h���ĂȂ�������t�F�[�h�A�E�g�J�n
	if (pPlayer_->GameOverUpdate() && !IsFadeing())
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
	DrawString(0, 0, "MainScene", 0xffffff, true);

	// �e�N���X�̕`��
//	pSkyDome_->Draw();
	pEnemyManager_->Draw();
	pPlayer_->Draw();

	// �t�F�[�h�̕`��
	DrawFade(true);

	// ���U�C�N�t�F�[�h�̕`��
	DrawGaussFade(true);
}