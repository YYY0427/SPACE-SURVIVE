#include "PauseScene.h"
#include "SceneManager.h"
#include "OptionScene.h"
#include "DebugScene.h"
#include "TitleScene.h"
#include "../Util/InputState.h"
#include "../common.h"
#include <DxLib.h>

namespace
{
	// �|�[�Y�E�B���h�E�g�̕�
	constexpr int window_width = common::screen_width;

	// �|�[�Y�E�B���h�E�g�̍���
	constexpr int window_height = common::screen_height;	

	// �|�[�Y�E�B���h�E�̍���̈ʒu(�E�B���h�E�̊J�n�ʒu)
	constexpr int window_start_x = 0;		
	constexpr int window_start_y = 0;	

	// �\������e�L�X�g�̑S�̂̈ʒu
	constexpr int draw_text_pos_x = common::screen_width / 2 - 100;
	constexpr int draw_text_pos_y = common::screen_height / 2 - 100;

	// �e�L�X�g�̕�����
	constexpr int text_space_y = 32;
}

// �R���X�g���N�^
PauseScene::PauseScene(SceneManager& manager) :
	Scene(manager),
	currentSelectItem_(0)
{
	// �t�F�[�h���s��Ȃ��悤�t�F�[�h���ĂȂ���ԂŊJ�n
	fadeBright_ = 0;
}

// �f�X�g���N�^
PauseScene::~PauseScene()
{
	// �����Ȃ�
}

// �X�V
void PauseScene::Update()
{
	

	// �I�������񂷏���
	// �t�F�[�h���̏ꍇ�͏������s��Ȃ�
	if (InputState::IsTriggered(InputType::UP) && !IsFadeing())
	{
		currentSelectItem_ = ((currentSelectItem_ - 1) + static_cast<int>(Item::TOTAL_VALUE)) % static_cast<int>(Item::TOTAL_VALUE);
	}
	else if (InputState::IsTriggered(InputType::DOWN) && !IsFadeing())
	{
		currentSelectItem_ = (currentSelectItem_ + 1) % static_cast<int>(Item::TOTAL_VALUE);
	}
	// ����{�^������������t�F�[�h�A�E�g�J�n
	if (InputState::IsTriggered(InputType::DECISION) && !IsFadeing())
	{
		// �t�F�[�h�A�E�g�J�n
	//	StartFadeOut();

		// �t�F�[�h�A�E�g���s��ꂽ���ǂ����̃t���O�𗧂Ă�
		// �V�[���J�ڂ̍ہA�t�F�[�h�A�E�g���s��ꂽ���ǂ������m�F���邽��
		isFadeOut_ = true;
	}
	// �t�F�[�h�A�E�g���I��肵�����I�����ꂽ���ڂɔ��
	if (isFadeOut_ && !IsFadingOut())
	{
		if (currentSelectItem_ == static_cast<int>(Item::DEBUG_SCENE))
		{
			manager_.PopAllSceneAndChangeScene(new DebugScene(manager_));
			return;
		}
		if (currentSelectItem_ == static_cast<int>(Item::CONFIG_SCENE))
		{
			isFadeOut_ = false;
			manager_.PushScene(new OptionScene(manager_));
			return;
		}
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

// �`��
void PauseScene::Draw()
{
	// �|�[�Y�E�B���h�E
	DrawBox(window_start_x, window_start_y, window_start_x + window_width, window_start_y + window_height, 0xd0d0d0, true);
	DrawBox(window_start_x, window_start_y, window_start_x + window_width, window_start_y + window_height, 0xffffff, false);

	// ���݂̃V�[���̃e�L�X�g�\��
	DrawString(window_start_x + 10, window_start_y + 10, "PauseScene", 0xffff88);

	// �f�o�b�O�V�[�������ׂ鍀�ڂ̃e�L�X�g�\��
	DrawString(draw_text_pos_x, draw_text_pos_y + text_space_y * static_cast<int>(Item::DEBUG_SCENE), "DebugScene", 0x000000, true);
	DrawString(draw_text_pos_x, draw_text_pos_y + text_space_y * static_cast<int>(Item::CONFIG_SCENE), "ConfigScene", 0x000000, true);

	// ���ݑI�𒆂̍��ڂ̉��Ɂ���\��
	DrawString(draw_text_pos_x - 32, draw_text_pos_y + text_space_y * currentSelectItem_, "��", 0xff0000);

	// �t�F�[�h�̕`��
	DrawFade();
}