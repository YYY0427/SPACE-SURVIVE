#include "ConfigScene.h"
#include "SceneManager.h"
#include "DebugScene.h"
#include "../Util/InputState.h"
#include "../Util/SoundManager.h"
#include "../Util/SaveData.h"
#include "../common.h"
#include <DxLib.h>

namespace
{
	// �\������e�L�X�g�̑S�̂̈ʒu
	constexpr int draw_text_pos_x = common::screen_width / 2 - 100;
	constexpr int draw_text_pos_y = common::screen_height / 2 - 100;

	// �e�L�X�g�̕�����
	constexpr int text_space = 32;
}

// �R���X�g���N�^
ConfigScene::ConfigScene(SceneManager& manager) :
	Scene(manager),
	currentSelectItem_(0)
{
	// �t�F�[�h���s��Ȃ��悤�t�F�[�h���ĂȂ���ԂŊJ�n
	fadeBright_ = 0;

	// BGM��炷
	SoundManager::GetInstance().PlayBGM("bgmTest");
}

// �f�X�g���N�^
ConfigScene::~ConfigScene()
{
	// �����Ȃ�
}

// �X�V
void ConfigScene::Update()
{
	// �I�������񂷏���
	// �t�F�[�h���̏ꍇ�͏������s��Ȃ�
	int itemTotalValue = static_cast<int>(Item::TOTAL_VALUE);
	if (InputState::IsTriggered(InputType::UP) && !IsFadeing())
	{
		currentSelectItem_ = ((currentSelectItem_ - 1) + itemTotalValue) % itemTotalValue;
	}
	else if (InputState::IsTriggered(InputType::DOWN) && !IsFadeing())
	{
		currentSelectItem_ = (currentSelectItem_ + 1) % itemTotalValue;
	}

	// �I������Ă���^�C�v�̉��ʂ̒���
	if (InputState::IsTriggered(InputType::RIGHT))
	{
		if (currentSelectItem_ == static_cast<int>(Item::BGM))
		{
			// BGM���ʃR���t�B�O�̑���
			// ���ʂ̍ő�l���傫���Ȃ�����ŏ��l�ɂ���
			SaveData::GetInstance().SetBgmVolume();

			// �ݒ肵���R���t�B�O���特�ʒ���
			SoundManager::GetInstance().SetVolume("bgmTest", 255);
		}
		else if (currentSelectItem_ == static_cast<int>(Item::SE))
		{
			// SE���ʃR���t�B�O�̑���
			// ���ʂ̍ő�l���傫���Ȃ�����ŏ��l�ɂ���
			SaveData::GetInstance().SetSeVolume();

			// SE�͕ύX���邽�тɃT�E���h��炷
			// �ݒ肵���R���t�B�O���特�ʒ���
			SoundManager::GetInstance().Play("cursorTest");
		}
	}

	// �O�̉�ʂɖ߂�
	if (InputState::IsTriggered(InputType::BACK))
	{
		// �V�[���J�ڂ���̂ŃT�E���h���~�߂�
		SoundManager::GetInstance().StopAllSound();

#ifdef _DEBUG
		manager_.PopScene();
#else 

#endif
		return;
	}
	// �t�F�[�h�̍X�V
	UpdateFade();
}

// �`��
void ConfigScene::Draw()
{
	DrawBox(0, 0, common::screen_width, common::screen_height, 0xd0d0d0, true);

	// ���݂̃V�[���̃e�L�X�g�\��
	DrawString(0, 0, "ConfigScene", 0x000000, true);

	// BGM�̍��ڂƉ��ʂ̕\��
	int bgm = static_cast<int>(Item::BGM);
	DrawString(draw_text_pos_x, draw_text_pos_y + text_space * bgm, "BGM", 0x000000, true);
	DrawFormatString(draw_text_pos_x + 32, draw_text_pos_y + text_space * bgm, 0x000000, "%d", SaveData::GetInstance().GetBgmVolume());

	// SE�̍��ڂƉ��ʂ̕\��
	int se = static_cast<int>(Item::SE);
	DrawString(draw_text_pos_x, draw_text_pos_y + text_space * se, "SE", 0x000000, true);
	DrawFormatString(draw_text_pos_x + 32, draw_text_pos_y + text_space * se, 0x000000, "%d", SaveData::GetInstance().GetSeVolume());

	// ���ݑI�𒆂̍��ڂ̉��Ɂ���\��
	DrawString(draw_text_pos_x - 32, draw_text_pos_y + text_space * currentSelectItem_, "��", 0xff0000);

	// �t�F�[�h�̕`��
	DrawFade();
}