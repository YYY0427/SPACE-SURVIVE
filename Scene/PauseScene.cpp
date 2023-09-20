#include "PauseScene.h"
#include "SceneManager.h"
#include "SoundSettingScene.h"
#include "DebugScene.h"
#include "TitleScene.h"
#include "../Util/InputState.h"
#include "../Game.h"
#include <DxLib.h>

namespace
{
	// �|�[�Y�E�B���h�E
	constexpr int window_width = Game::screen_width;	//�|�[�Y�g�̕�
	constexpr int window_height = Game::screen_height;	//�|�[�Y�g�̍���
	constexpr int window_start_x = (Game::screen_width - window_width) / 2;		//�|�[�Y�g�̍�
	constexpr int window_start_y = (Game::screen_height - window_height) / 2;	//�|�[�Y�g��

	// �\������e�L�X�g�̑S�̂̈ʒu
	constexpr int draw_text_pos_x = Game::screen_width / 2 - 100;
	constexpr int draw_text_pos_y = Game::screen_height / 2 - 100;

	// �e�L�X�g�̕�����
	constexpr int text_space = 32;
}

/// <summary>
/// �R���X�g���N�^
/// </summary>
/// <param name="manager">�V�[���}�l�[�W���[�̎Q��</param>
PauseScene::PauseScene(SceneManager& manager) :
	Scene(manager),
	currentSelectItem_(0)
{
	fadeBright_ = 0;
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
PauseScene::~PauseScene()
{
}

/// <summary>
/// �X�V
/// </summary>
void PauseScene::Update()
{
	// �t�F�[�h�A�E�g���I��肵�����I�����ꂽ�V�[���ɔ��
	if (isFadeOut_ && !IsFadingOut())
	{
		if (currentSelectItem_ == static_cast<int>(Item::DEBUG_SCENE))
		{
			manager_.PopAllSceneAndChangeScene(new DebugScene(manager_));
			return;
		}
	}

	// �I�������񂷏���
	if (InputState::IsTriggered(InputType::UP) && !isFadeOut_)
	{
		currentSelectItem_ = ((currentSelectItem_ - 1) + static_cast<int>(Item::NUM)) % static_cast<int>(Item::NUM);
	}
	else if (InputState::IsTriggered(InputType::DOWN) && !isFadeOut_)
	{
		currentSelectItem_ = (currentSelectItem_ + 1) % static_cast<int>(Item::NUM);
	}

	// ����{�^������������t�F�[�h�A�E�g�J�n
	if (InputState::IsTriggered(InputType::DECISION) && !IsFadingIn())
	{
		StartFadeOut();
		isFadeOut_ = true;
	}

	// �|�[�Y��ʂ���߂�
	if (InputState::IsTriggered(InputType::BACK) || InputState::IsTriggered(InputType::PAUSE))
	{
		manager_.PopScene();
		return;
	}

	// �t�F�[�h�̍X�V
	UpdateFade();
}

/// <summary>
/// �`��
/// </summary>
void PauseScene::Draw()
{
	//�|�[�Y�E�B���h�E
	DrawBox(window_start_x, window_start_y, window_start_x + window_width, window_start_y + window_height, 0xd0d0d0, true);
	DrawBox(window_start_x, window_start_y, window_start_x + window_width, window_start_y + window_height, 0xffffff, false);

	//�|�[�Y�����b�Z�[�W
	DrawString(window_start_x + 10, window_start_y + 10, "Pausing...", 0xffff88);

	DrawString(draw_text_pos_x, draw_text_pos_y + text_space * static_cast<int>(Item::DEBUG_SCENE), "DebugScene", 0x000000, true);
	DrawString(draw_text_pos_x - 32, draw_text_pos_y + text_space * currentSelectItem_, "��", 0xff0000);

	// �t�F�[�h�̕`��
	DrawFade();
}