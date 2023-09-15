#include "TestScene.h"
#include "SceneManager.h"
#include "DebugScene.h"
#include "../Util/Effekseer3DEffectManager.h"
#include "../Util/SoundManager.h"
#include "../Util/InputState.h"
#include "../Camera.h"
#include "../Player.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
TestScene::TestScene(SceneManager& manager) :
	Scene(manager)
{
	auto& effect = Effekseer3DEffectManager::GetInstance();
	pCamera_ = make_shared<Camera>();
	pPlayer_ = make_shared<Player>();
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
	// �t�F�[�h�A�E�g���I��莟��V�[���J��
	if (isFadeOut_ && !IsFadingOut())
	{
		manager_.ChangeScene(new DebugScene(manager_));
		return;
	}

	pCamera_->Update();
	pPlayer_->Update();

	// �߂�{�^����������ăt�F�[�h�C�����ĂȂ�������t�F�[�h�A�E�g�J�n
	if (InputState::IsTriggered(InputType::BACK) && !IsFadingIn())
	{
		StartFadeOut();

		// �t�F�[�h�̐ݒ�̕ύX
		SetFadeConfig(3, VGet(255, 255, 255), GetFadeBright());

		isFadeOut_ = true;
	}
	// �t�F�[�h�̍X�V
	UpdateFade();
}

/// <summary>
/// �`��
/// </summary>
void TestScene::Draw()
{
	DrawString(0, 0, "TestScene", 0xffffff, true);

	VECTOR pos1;
	VECTOR pos2;
	float lineAreaSize = 10000.0f;
	int lineNum = 50;

	pos1 = VGet(-lineAreaSize / 2.0f, 0.0f, -lineAreaSize / 2.0f);
	pos2 = VGet(-lineAreaSize / 2.0f, 0.0f, lineAreaSize / 2.0f);
	for (int i = 0; i <= lineNum; i++)
	{
		DrawLine3D(pos1, pos2, GetColor(255, 255, 255));
		pos1.x += lineAreaSize / lineNum;
		pos2.x += lineAreaSize / lineNum;
	}

	pos1 = VGet(-lineAreaSize / 2.0f, 0.0f, -lineAreaSize / 2.0f);
	pos2 = VGet(lineAreaSize / 2.0f, 0.0f, -lineAreaSize / 2.0f);
	for (int i = 0; i < lineNum; i++)
	{
		DrawLine3D(pos1, pos2, GetColor(255, 255, 255));
		pos1.z += lineAreaSize / lineNum;
		pos2.z += lineAreaSize / lineNum;
	}
	pPlayer_->Draw();

	// �t�F�[�h�̕`��
	DrawFade();
}