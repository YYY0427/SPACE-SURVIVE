#include "SoundSettingScene.h"
#include "SceneManager.h"
#include "../InputState.h"
#include "../Util/SoundManager.h"
#include "../Game.h"
#include <DxLib.h>

namespace
{
	// �E�B���h�E�^�C�g��
	const char* const window_title = "SoundSetting";

	// �{�����[�������ɕ����邩
	constexpr int volume_divide_num = 10;
	
	// ���ʐݒ�E�B���h�E
	constexpr int window_width = 700;												// �I�v�V�����g�̕�
	constexpr int window_height = 500;												// �I�v�V�����g�̍���
	constexpr int window_start_x = (Game::screen_width / 2) - (window_width / 2) + 50;	// �I�v�V�����g�̍�
	constexpr int window_start_y = (Game::screen_height / 2) - (window_height / 2) + 50;	// �I�v�V�����g��

	// �I�����̐�
	constexpr int name_count = 2;

	// �����̒l���ʂ�ω������邩
	constexpr int volume_setting_num = 25;
}

SoundSettingScene::SoundSettingScene(SceneManager& manager) :
	Scene(manager)
{
}

SoundSettingScene::~SoundSettingScene()
{
}

void SoundSettingScene::Update(const InputState& input)
{
	//�㉺�ŉ�鏈��
	if (input.IsTriggered(InputType::up))
	{
	//	SoundManager::GetInstance().Play("cursor");
		currentInputIndex_ = ((currentInputIndex_ - 1) + name_count) % name_count;
	}
	else if (input.IsTriggered(InputType::down))
	{
	//	SoundManager::GetInstance().Play("cursor");
		currentInputIndex_ = (currentInputIndex_ + 1) % name_count;
	}
	if (currentInputIndex_ == 0)
	{
		bgmCategoryColor = 0xff0000;
		seCategoryColor = 0x000000;
	}
	else 
	{
		bgmCategoryColor = 0x000000;
		seCategoryColor = 0xff0000;
	}

	//auto& soundMgr = SoundManager::GetInstance();
	//// BGM
	//if (input.IsTriggered(InputType::right) && currentInputIndex_ == 0)
	//{
	//	soundMgr.SetBGMVolume((std::min)(soundMgr.GetBGMVolume() + volume_setting_num, 255));
	//}
	//if (input.IsTriggered(InputType::left) && currentInputIndex_ == 0)
	//{
	//	soundMgr.SetBGMVolume((std::max)(soundMgr.GetBGMVolume() - volume_setting_num, 0));
	//}
	//// SE
	//if (input.IsTriggered(InputType::right) && currentInputIndex_ == 1)
	//{
	//	soundMgr.SetSEVolume((std::min)(soundMgr.GetSEVolume() + volume_setting_num, 255));
	//}
	//if (input.IsTriggered(InputType::left) && currentInputIndex_ == 1)
	//{
	//	soundMgr.SetSEVolume((std::max)(soundMgr.GetSEVolume() - volume_setting_num, 0));
	//}

	// �^�C�g����ʂɖ߂�
	if (input.IsTriggered(InputType::prev))
	{
		manager_.PopScene();
	}
}

void SoundSettingScene::Draw()
{
	//auto& soundMgr = SoundManager::GetInstance();

	//// ���ʂ̐��̎擾
	//int bgmNum = soundMgr.GetBGMVolume() / volume_setting_num;
	//int seNum = soundMgr.GetSEVolume() / volume_setting_num;

	//// �E�B���h�E
	//DrawBox(window_start_x, window_start_y, window_start_x + window_width, window_start_y + window_height, GetColor(200, 200, 200), true);

	//// �E�B���h�E�^�C�g���̕\��
	//int width = GetDrawStringWidth(window_title, static_cast<int>(strlen(window_title)), false);
	//DrawString(((window_width + window_start_x) / 2) - (width / 2), window_start_y + 50, window_title, 0x000000, false);

	//// ���ږ��̕\��
	//DrawString(window_start_x + window_width / 2 - 250, 320, "BGM", bgmCategoryColor, false);
	//DrawString(window_start_x + window_width / 2 - 250, 450, "SE", seCategoryColor, false);

	//for (int i = 0; i < volume_divide_num; i++)
	//{
	//	int width = i * 40;

	//	// �g�g��
	//	DrawRoundRect(window_start_x + window_width / 2 - 150 + width, 320, window_start_x + window_width / 2 - 150 + 30 + width, 320 + 80, 5, 5, GetColor(255, 165, 0), false);
	//	DrawRoundRect(window_start_x + window_width / 2 - 150 + width, 450, window_start_x + window_width / 2 - 150 + 30 + width, 450 + 80, 5, 5, GetColor(255, 165, 0), false);
	//}

	//// BGM
	//for (int i = 0; i < bgmNum; i++)
	//{
	//	int width = i * 40;
	//	DrawRoundRect(window_start_x + window_width / 2 - 150 + width, 320, window_start_x + window_width / 2 - 150 + 30 + width, 320 + 80, 5, 5, GetColor(255, 165, 0), true);
	//}
	//// SE
	//for (int i = 0; i < seNum; i++)
	//{
	//	int width = i * 40;
	//	DrawRoundRect(window_start_x + window_width / 2 - 150 + width, 450, window_start_x + window_width / 2 - 150 + 30 + width, 450 + 80, 5, 5, GetColor(255, 165, 0), true);
	//}
}