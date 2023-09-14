#include "TestScene.h"
#include "SceneManager.h"
#include "DebugScene.h"
#include "../Util/Effekseer3DEffectManager.h"
#include "../Util/SoundManager.h"
#include "../InputState.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
TestScene::TestScene(SceneManager& manager) :
	Scene(manager)
{
	auto& effect = Effekseer3DEffectManager::GetInstance();
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
void TestScene::Update(const InputState& input)
{
	// �t�F�[�h�A�E�g���I��莟��V�[���J��
	if (isFadeOut_ && !IsFadingOut())
	{
		manager_.ChangeScene(new DebugScene(manager_));
		return;
	}

	// �߂�{�^����������ăt�F�[�h�C�����ĂȂ�������t�F�[�h�A�E�g�J�n
	if (input.IsTriggered(InputType::BACK) && !IsFadingIn())
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

	// �t�F�[�h�̕`��
	DrawFade();
}