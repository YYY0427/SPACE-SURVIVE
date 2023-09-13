#include "DebugScene.h"
#include "SceneManager.h"
#include "TitleScene.h"
#include "MainScene.h"
#include "PauseScene.h"
#include "KeyConfigScene.h"
#include "SoundSettingScene.h"
#include "TestScene.h"
#include "../InputState.h"
#include "../Game.h"

namespace
{
	constexpr int draw_text_pos_x = Game::screen_width / 2 - 100;
	constexpr int draw_text_pos_y = Game::screen_height / 2 - 100;
	constexpr int text_space = 32;
}

/// <summary>
/// �R���X�g���N�^
/// </summary>
/// <param name="manager">�V�[���}�l�[�V���[�ւ̎Q��</param>
DebugScene::DebugScene(SceneManager& manager):
	Scene(manager),
	currentSelectIndex_(0)
{
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
DebugScene::~DebugScene()
{
}

/// <summary>
/// �X�V
/// </summary>
void DebugScene::Update(const InputState& input)
{
	// �t�F�[�h�A�E�g���I��肵�����I�����ꂽ�V�[���ɔ��
	if (isFadeOut_ && !IsFadingOut())
	{
		if (currentSelectIndex_ == TEST_SCENE)
		{
			manager_.ChangeScene(new TestScene(manager_));
			return;
		}
		else if (currentSelectIndex_ == TITLE_SCENE)
		{
			manager_.ChangeScene(new TitleScene(manager_));
			return;
		}
		else if (currentSelectIndex_ == MAIN_SCENE)
		{
			manager_.ChangeScene(new MainScene(manager_));
			return;
		}
	}

	// �I�������񂷏���
	if (input.IsTriggered(InputType::up) && !isFadeOut_)
	{
		currentSelectIndex_ = ((currentSelectIndex_ - 1) + NUM) % NUM;
	}
	else if (input.IsTriggered(InputType::down) && !isFadeOut_)
	{
		currentSelectIndex_ = (currentSelectIndex_ + 1) % NUM;
	}

	// ����{�^������������t�F�[�h�A�E�g�J�n
	if (input.IsTriggered(InputType::next) && !IsFadingIn())
	{
		StartFadeOut();
		isFadeOut_ = true;
	}

	// �t�F�[�h�̍X�V
	UpdateFade();
}

/// <summary>
///  �`��
/// </summary>
void DebugScene::Draw()
{
	// �e�L�X�g�̕\��
	DrawString(0, 0, "DebugScene", 0xffffff, true);
	DrawString(draw_text_pos_x, draw_text_pos_y + text_space * TEST_SCENE, "TestScene", 0xffffff, true);
	DrawString(draw_text_pos_x, draw_text_pos_y + text_space * TITLE_SCENE, "TitleScene", 0xffffff, true);
	DrawString(draw_text_pos_x, draw_text_pos_y + text_space * MAIN_SCENE, "MainScene", 0xffffff, true);
	DrawString(draw_text_pos_x - 32, draw_text_pos_y + text_space * currentSelectIndex_, "��", 0xff0000);

	// �t�F�[�h�̕`��
	DrawFade();
}