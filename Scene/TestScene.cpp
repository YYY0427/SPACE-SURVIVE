#include "TestScene.h"
#include "SceneManager.h"
#include "DebugScene.h"
#include "../Util/Effekseer3DEffectManager.h"
#include "../Util/SoundManager.h"
#include "../Util/InputState.h"
#include "../Camera.h"
#include "../Player.h"
#include "../SkyDome.h"
#include "../EnemyManager.h"
#include "../Enemy.h"
#include "../Game.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
TestScene::TestScene(SceneManager& manager) :
	Scene(manager),
	updateFunc_(&TestScene::NormalUpdate)
{
	auto& effect = Effekseer3DEffectManager::GetInstance();
	pPlayer_ = std::make_shared<Player>();
	pCamera_ = std::make_shared<Camera>(*pPlayer_);
	pSkyDome_ = std::make_shared<SkyDome>();
	pEnemyManager_ = std::make_shared<EnemyManager>(*pPlayer_);
	pPlayer_->SetCamera(pCamera_);
}

/// <summary>
///  �f�X�g���N�^
/// </summary>
TestScene::~TestScene()
{
}

/// <summary>
/// �X�V
/// </summary>
void TestScene::Update()
{
	(this->*updateFunc_)();
}

/// <summary>
/// �`��
/// </summary>
void TestScene::Draw()
{
	DrawBox(0, 0, Game::screen_width, Game::screen_height, 0xffffff, true);
	DrawString(0, 0, "TestScene", 0xffffff, true);

	VECTOR pos1;
	VECTOR pos2;
	float lineAreaSize = 10000.0f;
	int lineNum = 50;

	pos1 = VGet(-lineAreaSize / 2.0f, 0.0f, -lineAreaSize / 2.0f);
	pos2 = VGet(-lineAreaSize / 2.0f, 0.0f, lineAreaSize / 2.0f);
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
//	pSkyDome_->Draw();
	pEnemyManager_->Draw();
	pPlayer_->Draw();

	// �t�F�[�h�̕`��
	DrawFade();
}

void TestScene::NormalUpdate()
{
	// �t�F�[�h�A�E�g���I��莟��V�[���J��
	/*if (isFadeOut_ && !IsFadingOut())
	{
		manager_.ChangeScene(new DebugScene(manager_));
		return;
	}*/

	pCamera_->Update();
	pPlayer_->Update();
	//	pSkyDome_->Update();
	pEnemyManager_->Update();

	for (auto& enemies : pEnemyManager_->GetEnemies())
	{
		MV1_COLL_RESULT_POLY_DIM result = MV1CollCheck_Sphere(enemies->GetModelHandle(), -1, pPlayer_->GetPos(), 30.0f);
		if (result.HitNum > 0)
		{
			updateFunc_ = &TestScene::GameOverUpdate;
		}
	}

	// �߂�{�^����������ăt�F�[�h�C�����ĂȂ�������t�F�[�h�A�E�g�J�n
	//if (InputState::IsTriggered(InputType::BACK) && !IsFadingIn())
	//{
	//	StartFadeOut();

	//	// �t�F�[�h�̐ݒ�̕ύX
	//	SetFadeConfig(3, VGet(255, 255, 255), GetFadeBright());

	//	isFadeOut_ = true;
	//}
	// �t�F�[�h�̍X�V
	UpdateFade();
}

void TestScene::GameOverUpdate()
{
	// �t�F�[�h�A�E�g���I��莟��V�[���J��
	if (isFadeOut_ && !IsFadingOut())
	{
		manager_.ChangeScene(new DebugScene(manager_));
		return;
	}
	pCamera_->Update();
	bool a = pPlayer_->GameOverUpdate();

	// �߂�{�^����������ăt�F�[�h�C�����ĂȂ�������t�F�[�h�A�E�g�J�n
	if (a && !IsFadingIn())
	{
		StartFadeOut();

		// �t�F�[�h�̐ݒ�̕ύX
		SetFadeConfig(3, VGet(255, 255, 255), GetFadeBright());

		isFadeOut_ = true;
	}
	// �t�F�[�h�̍X�V
	UpdateFade();
}