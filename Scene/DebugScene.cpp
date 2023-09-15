#include "DebugScene.h"
#include "SceneManager.h"
#include "TitleScene.h"
#include "MainScene.h"
#include "PauseScene.h"
#include "SoundSettingScene.h"
#include "TestScene.h"
#include "../Util/InputState.h"
#include "../Game.h"

namespace
{
	// �\������e�L�X�g�̑S�̂̈ʒu
	constexpr int draw_text_pos_x = Game::screen_width / 2 - 100;
	constexpr int draw_text_pos_y = Game::screen_height / 2 - 100;

	// �e�L�X�g�̕�����
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
void DebugScene::Update()
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
		else if (currentSelectIndex_ == SOUNDSETTING_SCENE)
		{
			manager_.ChangeScene(new SoundSettingScene(manager_));
			return;
		}
		else if (currentSelectIndex_ == PAUSE_SCENE)
		{
			// �|�[�Y�̏ꍇ�V�[�����c���Ă���̂ŏ�����
			isFadeOut_ = false;

			manager_.PushScene(new PauseScene(manager_));
			return;
		}
	}

	// �I�������񂷏���
	if (InputState::IsTriggered(InputType::UP) && !isFadeOut_)
	{
		currentSelectIndex_ = ((currentSelectIndex_ - 1) + NUM) % NUM;
	}
	else if (InputState::IsTriggered(InputType::DOWN) && !isFadeOut_)
	{
		currentSelectIndex_ = (currentSelectIndex_ + 1) % NUM;
	}

	// ����{�^������������t�F�[�h�A�E�g�J�n
	if (InputState::IsTriggered(InputType::DECISION) && !IsFadingIn())
	{
		// �|�[�Y�̏ꍇ�̓t�F�[�h���s��Ȃ�
		if (currentSelectIndex_ != PAUSE_SCENE)
		{
			StartFadeOut();
		}
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
	DrawString(draw_text_pos_x, draw_text_pos_y + text_space * SOUNDSETTING_SCENE, "SoundSettingScene", 0xffffff, true);
	DrawString(draw_text_pos_x, draw_text_pos_y + text_space * PAUSE_SCENE, "PauseScene", 0xffffff, true);

	DrawString(draw_text_pos_x - 32, draw_text_pos_y + text_space * currentSelectIndex_, "��", 0xff0000);

	// �t�F�[�h�̕`��
	DrawFade();
}