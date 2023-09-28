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
	handle_ = LoadGraph("Data/ocean.jpg");
}

// �f�X�g���N�^
MainScene::~MainScene()
{
	DeleteGraph(handle_);
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
		// �t�F�[�h�A�E�g��ʏ�̔����Ŏ~�߂�
		StartFadeOut(255 / 2);
	}
	// �t�F�[�h���I��莟��V�[���J��
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
	DrawGraph(0, 0, handle_, true);

	// ���݂̃V�[���̃e�L�X�g�\��
	DrawString(0, 0, "MainScene", 0xffffff, true);

	// �t�F�[�h�̕`��
	DrawFade(true);

	// ���U�C�N�t�F�[�h�̕`��
	DrawGaussFade(true);
}