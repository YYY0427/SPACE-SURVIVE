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
		manager_.PushScene(new PauseScene(manager_));
		return;
	}
	//// �߂�{�^����������A�t�F�[�h������Ȃ��ꍇ�t�F�[�h�A�E�g�J�n
	//if (InputState::IsTriggered(InputType::BACK) && !IsFadeing())
	//{
	//	// �t�F�[�h�A�E�g�J�n
	//	StartFadeOut();

	//	// �t�F�[�h�A�E�g���s��ꂽ���ǂ����̃t���O�𗧂Ă�
	//	// �V�[���J�ڂ̍ہA�t�F�[�h�A�E�g���s��ꂽ���ǂ������m�F���邽��
	//	isFadeOut_ = true;
	//}
	//// �t�F�[�h�A�E�g���I��莟��V�[���J��
	//if (isFadeOut_ && !IsFadingOut())
	//{
	//	manager_.ChangeScene(new TitleScene(manager_));
	//	return;
	//}
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
}