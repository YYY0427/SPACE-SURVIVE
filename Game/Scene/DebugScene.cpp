#include "DebugScene.h"
#include "SceneManager.h"
#include "TitleScene.h"
#include "MainScene.h"
#include "PauseScene.h"
#include "OptionScene.h"
#include "TestScene.h"
#include "../Util/InputState.h"
#include "../Util/Range.h"
#include "../Util/Debug.h"
#include "../common.h"

namespace
{
	// �\������e�L�X�g�̑S�̂̈ʒu
	constexpr int draw_text_pos_x = common::screen_width / 2 - 100;
	constexpr int draw_text_pos_y = common::screen_height / 2 - 100;

	// �e�L�X�g�̕�����
	constexpr int text_space_y = 32;
}

// �R���X�g���N�^
DebugScene::DebugScene(SceneManager& manager):
	SceneBase(manager),
	currentSelectItem_(0)
{
}

// �f�X�g���N�^
DebugScene::~DebugScene()
{
	// �����Ȃ�
}

// �X�V
void DebugScene::Update()
{
	// �I�������񂷏���
	int sceneItemTotalValue = static_cast<int>(Item::TOTAL_VALUE);
	if (InputState::IsTriggered(InputType::UP))
	{
		currentSelectItem_ = ((currentSelectItem_ - 1) + sceneItemTotalValue) % sceneItemTotalValue;
	}
	else if (InputState::IsTriggered(InputType::DOWN))
	{
		currentSelectItem_ = (currentSelectItem_ + 1) % sceneItemTotalValue;
	}

	// ����{�^����������āA�t�F�[�h���ł͂Ȃ�������t�F�[�h�A�E�g�J�n
	if (InputState::IsTriggered(InputType::DECISION))
	{
		// �t�F�[�h�A�E�g�J�n
		StartFadeOut(255);
	}

	// �t�F�[�h�A�E�g���I��肵�����I�����ꂽ�V�[���ɔ��
	if (IsStartFadeOutAfterFadingOut())
	{
		switch (static_cast<Item>(currentSelectItem_))
		{
		// �e�X�g�V�[��
		case Item::TEST_SCENE:
			manager_.ChangeScene(new TestScene(manager_));
			return;

		// �^�C�g���V�[��
		case Item::TITLE_SCENE:
			manager_.ChangeScene(new TitleScene(manager_));
			return;

		// ���C���V�[��
		case Item::MAIN_SCENE:
			manager_.ChangeScene(new MainScene(manager_));
			return;

		// �I�v�V�����V�[��
		case Item::OPTION_SCENE:
			manager_.PushScene(new OptionScene(manager_));
			break;

		// �|�[�Y�V�[��
		case Item::PAUSE_SCENE:
			manager_.PushScene(new PauseScene(manager_));
			break;

		// ���肦�Ȃ��̂Ŏ~�߂�
		default:
			assert(0);
		}

		// PushScene�����ꍇ�V�[�����c�����܂܂Ȃ̂�
		// �t�F�[�h�C���̐ݒ�
		StartFadeIn();
		return;
	}
	// �t�F�[�h�̍X�V
	UpdateFade();
}

//  �`��
void DebugScene::Draw()
{
	// ���݂̃V�[���̃e�L�X�g�\��
	Debug::Log("DebugScene");

	// �f�o�b�O�V�[�������ׂ�V�[���̍��ڂ̃e�L�X�g�\��
	DrawString(draw_text_pos_x, draw_text_pos_y + text_space_y * static_cast<int>(Item::TEST_SCENE), "TestScene", 0xffffff, true);
	DrawString(draw_text_pos_x, draw_text_pos_y + text_space_y * static_cast<int>(Item::TITLE_SCENE), "TitleScene", 0xffffff, true);
	DrawString(draw_text_pos_x, draw_text_pos_y + text_space_y * static_cast<int>(Item::MAIN_SCENE), "MainScene", 0xffffff, true);
	DrawString(draw_text_pos_x, draw_text_pos_y + text_space_y * static_cast<int>(Item::OPTION_SCENE), "ConfigScene", 0xffffff, true);
	DrawString(draw_text_pos_x, draw_text_pos_y + text_space_y * static_cast<int>(Item::PAUSE_SCENE), "PauseScene", 0xffffff, true);

	// ���ݑI�𒆂̍��ڂ̉��Ɂ���\��
	DrawString(draw_text_pos_x - text_space_y, draw_text_pos_y + text_space_y * currentSelectItem_, "��", 0xff0000);

	// �t�F�[�h�̕`��
	DrawFade(true);

	// ���U�C�N�t�F�[�h�̕`��
	DrawGaussFade(true);
}