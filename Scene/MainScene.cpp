#include "MainScene.h"
#include "TitleScene.h"
#include "PauseScene.h"
#include "SceneManager.h"
#include "../common.h"
#include "../Util/InputState.h"
#include "../Util/Effekseer3DEffectManager.h"
#include "../Util/SoundManager.h"
#include <DxLib.h>

// �R���X�g���N�^
MainScene::MainScene(SceneManager& manager) :
	Scene(manager),
	updateFunc_(&MainScene::NormalUpdate)
{
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
	
	// �|�[�Y��ʂɑJ��
	if (InputState::IsTriggered(InputType::PAUSE))
	{
		// �t�F�[�h�A�E�g�J�n
		StartFadeOut();
	}
	// �t�F�[�h�A�E�g���I��莟��V�[���J��
	if (IsStartFadeOutAfterFadingOut())
	{
		StartFadeIn();
		manager_.PushScene(new PauseScene(manager_));
		return;
	}
	// �t�F�[�h�̍X�V
	UpdateFade();
}

// �`��
void MainScene::Draw()
{
	// ���݂̃V�[���̃e�L�X�g�\��
	DrawString(0, 0, "MainScene", 0xffffff, true);

	// �t�F�[�h�̕`��
	DrawFade();

	// ���U�C�N�t�F�[�h�̕`��
	DrawGaussFade();
}