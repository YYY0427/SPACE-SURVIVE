#include "TestScene.h"
#include "TitleScene.h"
#include "PauseScene.h"
#include "SceneManager.h"
#include "../common.h"
#include "../Util/InputState.h"
#include "../Util/Effekseer3DEffectManager.h"
#include "../Util/SoundManager.h"
#include "../Util/Debug.h"
#include "../Camera.h"
#include "../UI/Warning.h"
#include <DxLib.h>

// �R���X�g���N�^
TestScene::TestScene(SceneManager& manager) :
	SceneBase(manager)
{
	pWarning_ = std::make_shared<Warning>();
}

// �f�X�g���N�^
TestScene::~TestScene()
{
	// �����Ȃ�
}

// �X�V
void TestScene::Update()
{
	// �e�N���X�̍X�V
	pWarning_->Update();

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

// �`��
void TestScene::Draw()
{
	// ���݂̃V�[���̃e�L�X�g�\��
	Debug::Log("TestScene");

	// �e�N���X�̕`��
	pWarning_->Draw();

	// �t�F�[�h�̕`��
	DrawFade(true);

	// ���U�C�N�t�F�[�h�̕`��
	DrawGaussFade(true);
}