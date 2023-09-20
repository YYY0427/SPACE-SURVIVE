#include "MainScene.h"
#include "TitleScene.h"
#include "PauseScene.h"
#include "SceneManager.h"
#include "DebugScene.h"
#include "../Game.h"
#include "../Util/InputState.h"
#include "../Util/Effekseer3DEffectManager.h"
#include "../Util/SoundManager.h"
#include <DxLib.h>

/// <summary>
/// �R���X�g���N�^
/// </summary>
/// <param name="manager">�V�[���}�l�[�W���[�ւ̎Q��</param>
MainScene::MainScene(SceneManager& manager) :
	Scene(manager),
	updateFunc_(&MainScene::NormalUpdate)
{
	
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
MainScene::~MainScene()
{
}

/// <summary>
/// �X�V
/// </summary>
void MainScene::Update()
{
	(this->*updateFunc_)();
}

/// <summary>
/// �ʏ�̍X�V
/// </summary>
void MainScene::NormalUpdate()
{
	// �t�F�[�h���I��莟��V�[���J��
	if (isFadeOut_ && !IsFadingOut())
	{
		manager_.ChangeScene(new DebugScene(manager_));
		return;
	}

	// �߂�{�^���������ꂽ��t�F�[�h�A�E�g�J�n
	if (InputState::IsTriggered(InputType::BACK) && !IsFadingIn())
	{
		StartFadeOut();
		isFadeOut_ = true;
	}
	// �|�[�Y��ʂɑJ��
	if (InputState::IsTriggered(InputType::PAUSE))
	{
		manager_.PushScene(new PauseScene(manager_));
		return;
	}
	// �t�F�[�h�̍X�V
	UpdateFade();
}

/// <summary>
/// �`��
/// </summary>
void MainScene::Draw()
{
	DrawString(0, 0, "MainScene", 0xffffff, true);

	// �t�F�[�h�̕`��
	DrawFade();
}