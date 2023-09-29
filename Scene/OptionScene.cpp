#include "OptionScene.h"
#include "SceneManager.h"
#include "DebugScene.h"
#include "PadSettingScene.h"
#include "../Util/InputState.h"
#include "../Util/SoundManager.h"
#include "../Util/SaveData.h"
#include "../Util/StringManager.h"
#include "../Util/DrawFunctions.h"
#include "../common.h"
#include <DxLib.h>
#include <array>

namespace
{
	// �\������e�L�X�g�̑S�̂̈ʒu
	constexpr int draw_text_pos_x = 350;
	constexpr int draw_text_pos_y = common::screen_height / 2 - 150;

	// �e�L�X�g�̕�����
	constexpr int text_space_y = 64;

	// �I������Ă��Ȃ��Ƃ��̕`��F
	constexpr int normal_color = 0x666666;

	// �I������Ă���Ƃ��̕`��F
	constexpr int choose_color = 0xffffff;
}

// �R���X�g���N�^
OptionScene::OptionScene(SceneManager& manager) :
	Scene(manager),
	currentSelectItem_(0),
	soundIconImgHandle_(-1)
{
	SetFadeBright(0);

	// �摜�̃��[�h
	soundIconImgHandle_ = my::MyLoadGraph("Data/Image/Sound.png");

	// ���ڂ̕`��F��I������Ă��Ȃ��Ƃ��̐F�ɏ�����
	for (int i = 0; i < static_cast<int>(Item::TOTAL_VALUE); i++)
	{
		itemColorDataTable_.push_back(normal_color);
	}
}

// �f�X�g���N�^
OptionScene::~OptionScene()
{
	DeleteGraph(soundIconImgHandle_);
}

// �X�V
void OptionScene::Update()
{
	// �J���[�̏�����
	for (auto& itemColor : itemColorDataTable_)
	{
		itemColor = normal_color;
	}

	// �I�������񂷏���
	int itemTotalValue = static_cast<int>(Item::TOTAL_VALUE);
	if (InputState::IsTriggered(InputType::UP))
	{
		currentSelectItem_ = ((currentSelectItem_ - 1) + itemTotalValue) % itemTotalValue;
	}
	else if (InputState::IsTriggered(InputType::DOWN))
	{
		currentSelectItem_ = (currentSelectItem_ + 1) % itemTotalValue;
	}

	// �I������Ă��鍀�ڂ̐F��ς���
	itemColorDataTable_[currentSelectItem_] = choose_color;

	switch (currentSelectItem_)
	{
	case static_cast<int>(Item::LANGUAGE):
		break;
	case static_cast<int>(Item::WINDOW_MODE):
		SaveData::GetInstance().SetWindowMode();
		break;
	case static_cast<int>(Item::MASTER_VOLUME):
		// �S�̉��ʂ̒���
		SaveData::GetInstance().SetMasterVolume();
		break;
	case static_cast<int>(Item::BGM_VOLUME):
		// BGM���ʂ̒���
		SaveData::GetInstance().SetBgmVolume();
		break;
	case static_cast<int>(Item::SE_VOLUME):
		// SE���ʂ̒���
		SaveData::GetInstance().SetSeVolume();
		break;
	case static_cast<int>(Item::PAD_SETTING):
		if(InputState::IsTriggered(InputType::DECISION)) StartFadeOut(255, 16);
		break;
	case static_cast<int>(Item::BACK):
		if (InputState::IsTriggered(InputType::DECISION)) manager_.PopScene();
		return;
	}
	
	if (IsStartFadeOutAfterFadingOut())
	{
		StartFadeIn();
		manager_.PushScene(new PadSettingScene(manager_));
		return;
	}

	if (InputState::IsTriggered(InputType::BACK))
	{
		manager_.PopScene();
		return;
	}

	// �t�F�[�h�̍X�V
	UpdateFade();
}

// �`��
void OptionScene::Draw()
{
	// �C���X�^���X�̎擾
	auto& stringManager = StringManager::GetInstance();
	auto& saveData = SaveData::GetInstance();

	// �`�擧���x�̐ݒ�
	int fade = 255 - GetFadeBright();
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, fade);

	// �V�[���^�C�g���̕`��
	stringManager.DrawStringCenter("OptionTitle", common::screen_width / 2, 100, 0xffffff);

	// ���ڂ̕`��
	int language = static_cast<int>(Item::LANGUAGE);
	int windowMode = static_cast<int>(Item::WINDOW_MODE);
	int masterVolume = static_cast<int>(Item::MASTER_VOLUME);
	int bgmVolume = static_cast<int>(Item::BGM_VOLUME);
	int seVolume = static_cast<int>(Item::SE_VOLUME);
	int padSetting = static_cast<int>(Item::PAD_SETTING);
	int back = static_cast<int>(Item::BACK);
	stringManager.DrawStringCenter("OptionItemLanguage", draw_text_pos_x, draw_text_pos_y + text_space_y * language, itemColorDataTable_[language]);
	stringManager.DrawStringCenter("OptionItemWindowMode", draw_text_pos_x, draw_text_pos_y + text_space_y * windowMode, itemColorDataTable_[windowMode]);
	stringManager.DrawStringCenter("OptionItemMasterVolume", draw_text_pos_x, draw_text_pos_y + text_space_y * masterVolume, itemColorDataTable_[masterVolume]);
	stringManager.DrawStringCenter("OptionItemBgmVolume", draw_text_pos_x, draw_text_pos_y + text_space_y * bgmVolume, itemColorDataTable_[bgmVolume]);
	stringManager.DrawStringCenter("OptionItemSeVolume", draw_text_pos_x, draw_text_pos_y + text_space_y * seVolume, itemColorDataTable_[seVolume]);
	stringManager.DrawStringCenter("OptionItemPadSetting", common::screen_width / 2, draw_text_pos_y + text_space_y * padSetting, itemColorDataTable_[padSetting]);
	stringManager.DrawStringCenter("OptionItemBack", common::screen_width / 2, draw_text_pos_y + text_space_y * back, itemColorDataTable_[back]);

	// �E�B���h�E���[�h�̏�Ԃ̕\��
	if (saveData.GetSaveData().windowMode)
	{
		stringManager.DrawStringCenter("OptionItemWindowModeOff", common::screen_width / 2, draw_text_pos_y + text_space_y * windowMode, itemColorDataTable_[windowMode]);
	}
	else
	{
		stringManager.DrawStringCenter("OptionItemWindowModeOn", common::screen_width / 2, draw_text_pos_y + text_space_y * windowMode, itemColorDataTable_[windowMode]);
	}

	// �e�T�E���h���ʂ̕\��
	std::array<int, 3> volume = {};
	volume[0] = SaveData::GetInstance().GetSaveData().masterVolume;
	volume[1] = SaveData::GetInstance().GetSaveData().bgmVolume;
	volume[2] = SaveData::GetInstance().GetSaveData().seVolume;
	int item = masterVolume;
	for(int i = 0; i < volume.size(); i++)
	{
		for (int j = 0; j < volume[i]; j++)
		{
			SetDrawBright(70, 70, 70);
			if (currentSelectItem_ == item)
			{
				SetDrawBright(255, 255, 255);
			}
			int textSpaceX = j * 70;
			DrawRotaGraph(draw_text_pos_x + 170 + textSpaceX, draw_text_pos_y + text_space_y * item + 10, 0.2, 0.0, soundIconImgHandle_, true);
		}
		item++;
	}
	SetDrawBright(255, 255, 255);

	// �`��̐ݒ�̏�����
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}