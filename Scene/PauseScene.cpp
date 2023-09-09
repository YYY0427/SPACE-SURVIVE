#include "PauseScene.h"
#include "SceneManager.h"
#include "KeyConfigScene.h"
#include "SoundSettingScene.h"
#include "../InputState.h"
#include "../Game.h"
#include <DxLib.h>

namespace
{
	// �|�[�Y�E�B���h�E
	constexpr int window_width = 700;	//�|�[�Y�g�̕�
	constexpr int window_height = 500;	//�|�[�Y�g�̍���
	constexpr int window_start_x = (Game::screen_width - window_width) / 2;		//�|�[�Y�g�̍�
	constexpr int window_start_y = (Game::screen_height - window_height) / 2;	//�|�[�Y�g��

	constexpr int name_count = 2;
}

PauseScene::PauseScene(SceneManager& manager) :
	Scene(manager)
{
}

PauseScene::~PauseScene()
{
}

void PauseScene::Update(const InputState& input)
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
		keyConfigCategoryColor_ = 0xff0000;
		soundSettingCategoryColor_ = 0x000000;
	}
	else
	{
		keyConfigCategoryColor_ = 0x000000;
		soundSettingCategoryColor_ = 0xff0000;
	}

	if (input.IsTriggered(InputType::next) && currentInputIndex_ == 0)
	{
		manager_.PushScene(new KeyConfigScene(manager_, input));
	}
	if (input.IsTriggered(InputType::next) && currentInputIndex_ == 1)
	{
		manager_.PushScene(new SoundSettingScene(manager_));
	}
	if (input.IsTriggered(InputType::pause))
	{
		manager_.PopScene();
		return;
	}
}

void PauseScene::Draw()
{
	SetDrawBlendMode(DX_BLENDMODE_MULA, 196);
	DrawBox(window_start_x, window_start_y, window_start_x + window_width, window_start_y + window_height, 0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	//�|�[�Y�E�B���h�E
	DrawBox(window_start_x, window_start_y, window_start_x + window_width, window_start_y + window_height, 0xd0d0d0, true);
	DrawBox(window_start_x, window_start_y, window_start_x + window_width, window_start_y + window_height, 0xffffff, false);

	//�|�[�Y�����b�Z�[�W
	DrawString(window_start_x + 10, window_start_y + 10, "Pausing...", 0xffff88);

	DrawString(window_start_x + 50, window_start_y + 50, "KeyConfig", keyConfigCategoryColor_);
	DrawString(window_start_x + 50, window_start_y + 70, "SoundSetting", soundSettingCategoryColor_);
}