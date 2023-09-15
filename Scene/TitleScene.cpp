#include <DxLib.h>
#include "TitleScene.h"
#include "SceneManager.h"
#include "MainScene.h"
#include "DebugScene.h"
#include "../Util/InputState.h"
#include "../Util/SoundManager.h"
#include "../Util/DrawFunctions.h"
#include "../Game.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
/// <param name="manager">�V�[���}�l�[�W���[�̎Q��</param>
TitleScene::TitleScene(SceneManager& manager) :
	Scene(manager),
	updateFunc_(&TitleScene::NormalUpdate)
{
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
TitleScene::~TitleScene()
{
}

/// <summary>
/// �X�V
/// </summary>
void TitleScene::Update()
{
	(this->*updateFunc_)();
}

/// <summary>
/// �ʏ�̍X�V
/// </summary>
void TitleScene::NormalUpdate()
{
	if (isFadeOut_ && !IsFadingOut())
	{
		manager_.ChangeScene(new DebugScene(manager_));
		return;
	}

	//���ւ̃{�^���������ꂽ�玟�̃V�[���֍s��
	if (InputState::IsTriggered(InputType::BACK) && !isFadeOut_)
	{
		isFadeOut_ = true;
		StartFadeOut();
	}
	UpdateFade();
}

/// <summary>
/// �`��
/// </summary>
void TitleScene::Draw()
{
	DrawString(0, 0, "TitleScene", 0xffffff, true);

	DrawFade();
}