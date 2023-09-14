#include "SoundSettingScene.h"
#include "SceneManager.h"
#include "DebugScene.h"
#include "../InputState.h"
#include "../Util/SoundManager.h"
#include "../Util/SaveData.h"
#include "../Game.h"
#include <DxLib.h>

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
/// <param name="manager">�V�[���}�l�[�W���[�̎Q��</param>
SoundSettingScene::SoundSettingScene(SceneManager& manager) :
	Scene(manager),
	currentSelectIndex_(0)
{
	SoundManager::GetInstance().PlayBGM("bgmTest");
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
SoundSettingScene::~SoundSettingScene()
{
}

/// <summary>
/// �X�V
/// </summary>
void SoundSettingScene::Update(const InputState& input)
{
	//�I�������񂷏���
	if (input.IsTriggered(InputType::UP))
	{
		currentSelectIndex_ = ((currentSelectIndex_ - 1) + NUM) % NUM;
	}
	else if (input.IsTriggered(InputType::DOWN))
	{
		currentSelectIndex_ = (currentSelectIndex_ + 1) % NUM;
	}

	// �I������Ă���^�C�v�̉��ʂ̒���
	if (input.IsTriggered(InputType::RIGHT))
	{
		if (currentSelectIndex_ == BGM)
		{
			SaveData::GetInstance().SetBgmVolume();
			SoundManager::GetInstance().SetVolume("bgmTest", 255);
		}
		else if (currentSelectIndex_ == SE)
		{
			SaveData::GetInstance().SetSeVolume();
			SoundManager::GetInstance().Play("cursorTest");
		}
	}

	// �O�̉�ʂɖ߂�
	if (input.IsTriggered(InputType::BACK))
	{
		// �V�[���J�ڂ���̂ŃT�E���h���~�߂�
		SoundManager::GetInstance().StopAllSound();

#ifdef _DEBUG
		manager_.ChangeScene(new DebugScene(manager_));
#else 
#endif
		return;
	}
}

void SoundSettingScene::Draw()
{
	DrawString(0, 0, "SoundSettingScene", 0xffffff, true);

	DrawString(draw_text_pos_x, draw_text_pos_y + text_space * BGM, "BGM", 0xffffff, true);
	DrawFormatString(draw_text_pos_x + 32, draw_text_pos_y + text_space * BGM, 0xffffff, "%d", SaveData::GetInstance().GetBgmVolume());

	DrawString(draw_text_pos_x, draw_text_pos_y + text_space * SE, "SE", 0xffffff, true);
	DrawFormatString(draw_text_pos_x + 32, draw_text_pos_y + text_space * SE, 0xffffff, "%d", SaveData::GetInstance().GetSeVolume());

	DrawString(draw_text_pos_x - 32, draw_text_pos_y + text_space * currentSelectIndex_, "��", 0xff0000);
}