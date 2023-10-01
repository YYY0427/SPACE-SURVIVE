#include "TestScene.h"
#include "SceneManager.h"
#include "DebugScene.h"
#include "PauseScene.h"
#include "../Util/Effekseer3DEffectManager.h"
#include "../Util/SoundManager.h"
#include "../Util/InputState.h"
#include "../Util/DataReaderFromUnity.h"
#include "../Camera.h"
#include "../Player.h"
#include "../SkyDome.h"
#include "../EnemyManager.h"
#include "../Enemy.h"
#include "../common.h"

// �R���X�g���N�^
TestScene::TestScene(SceneManager& manager) :
	Scene(manager),
	updateFunc_(&TestScene::NormalUpdate)
{
	// �C���X�^���X�쐬
	pDataReader_ = std::make_shared<DataReaderFromUnity>();
	pPlayer_ = std::make_shared<Player>();
	pCamera_ = std::make_shared<Camera>(pPlayer_);
//	pSkyDome_ = std::make_shared<SkyDome>(pPlayer_);
	pEnemyManager_ = std::make_shared<EnemyManager>(pPlayer_);

	// �R���X�g���N�^�œn���Ȃ��|�C���^�̐ݒ�
	pPlayer_->SetCameraPointer(pCamera_);

	pDataReader_->LoadUnityGameObjectData();
}

//  �f�X�g���N�^
TestScene::~TestScene()
{
	// �����Ȃ�
}

// �����o�֐��|�C���^�̍X�V
void TestScene::Update()
{
	(this->*updateFunc_)();
}

// �`��
void TestScene::Draw()
{
	// �w�i�𔒂ɕύX
	DrawBox(0, 0, common::screen_width, common::screen_height, 0xcccccc, true);

	// ���݂̃V�[���̃e�L�X�g�\��
	DrawString(0, 0, "TestScene", 0xffffff, true);

	// �e�N���X�̕`��
//	pSkyDome_->Draw();
	GroundLineDraw();
	pEnemyManager_->Draw();
	pPlayer_->Draw();

	int x = 200, y = 200;
	for (const auto& data : pDataReader_->GetData())
	{
		DrawFormatString(x, y, 0x000000, "%s = pos {%.2f, %.2f, %.2f}, rot {%.2f, %.2f, %.2f}", data.name.c_str(), data.pos.x, data.pos.y, data.pos.z, data.rot.x, data.rot.y, data.rot.z);
		y += 16;
	}

	// �t�F�[�h�̕`��
	DrawFade(true);

	DrawGaussFade(true);
}

// �ʏ�̍X�V
void TestScene::NormalUpdate()
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
			updateFunc_ = &TestScene::GameOverUpdate;
		}
	}

	// �|�[�Y��ʂɑJ��
	if (InputState::IsTriggered(InputType::PAUSE))
	{
		StartFadeOut(200, 64);
	}
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

// �Q�[���I�[�o�[���̍X�V
void TestScene::GameOverUpdate()
{
	// �t�F�[�h�A�E�g���I��莟��V�[���J��
	if (IsStartFadeOutAfterFadingOut())
	{
		manager_.ChangeScene(new DebugScene(manager_));
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

// �n�ʂ̐��̕`��
void TestScene::GroundLineDraw()
{
	float lineAreaSize = 10000.0f;
	int lineNum = 50;

	VECTOR pos1 = VGet(-lineAreaSize / 2.0f, 0.0f, -lineAreaSize / 2.0f);
	VECTOR pos2 = VGet(-lineAreaSize / 2.0f, 0.0f, lineAreaSize / 2.0f);
	for (int i = 0; i <= lineNum; i++)
	{
		DrawLine3D(pos1, pos2, GetColor(0, 0, 0));
		pos1.x += lineAreaSize / lineNum;
		pos2.x += lineAreaSize / lineNum;
	}

	pos1 = VGet(-lineAreaSize / 2.0f, 0.0f, -lineAreaSize / 2.0f);
	pos2 = VGet(lineAreaSize / 2.0f, 0.0f, -lineAreaSize / 2.0f);
	for (int i = 0; i < lineNum; i++)
	{
		DrawLine3D(pos1, pos2, GetColor(0, 0, 0));
		pos1.z += lineAreaSize / lineNum;
		pos2.z += lineAreaSize / lineNum;
	}
}