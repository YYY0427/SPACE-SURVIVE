#include <DxLib.h>
#include "TitleScene.h"
#include "SceneManager.h"
#include "MainScene.h"
#include "DebugScene.h"
#include "../Util/InputState.h"
#include "../Util/SoundManager.h"
#include "../Util/DrawFunctions.h"
#include "../common.h"

// �R���X�g���N�^
TitleScene::TitleScene(SceneManager& manager) :
	Scene(manager),
	updateFunc_(&TitleScene::NormalUpdate)
{
}

// �f�X�g���N�^
TitleScene::~TitleScene()
{
}

// �����o�֐��|�C���^�̍X�V
void TitleScene::Update()
{
	(this->*updateFunc_)();
}

// �ʏ�̍X�V
void TitleScene::NormalUpdate()
{


	// ���ւ̃{�^����������āA�t�F�[�h���łȂ�������t�F�[�h�A�E�g�̊J�n
	if (InputState::IsTriggered(InputType::DECISION) && !IsFadeing())
	{
		// �t�F�[�h�A�E�g�̊J�n
		StartFadeOut();

		// �t�F�[�h�A�E�g���s��ꂽ���ǂ����̃t���O�𗧂Ă�
		// �V�[���J�ڂ̍ہA�t�F�[�h�A�E�g���s��ꂽ���ǂ������m�F���邽��
		isFadeOut_ = true;
	}
	// �t�F�[�h�A�E�g���I��莟��V�[���J��
	if (isFadeOut_ && !IsFadingOut())
	{
		manager_.ChangeScene(new DebugScene(manager_));
		return;
	}
	// �t�F�[�h�̍X�V
	UpdateFade();
}

// �`��
void TitleScene::Draw()
{
	// ���݂̃V�[���̃e�L�X�g�\��
	DrawString(0, 0, "TitleScene", 0xffffff, true);

	// �t�F�[�h�̕`��
	DrawFade();
}