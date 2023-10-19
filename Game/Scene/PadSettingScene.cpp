#include "PadSettingScene.h"
#include "SceneManager.h"
#include "../Util/StringManager.h"
#include "../Util/InputState.h"
#include "../Util/SaveData.h"
#include "../common.h"
#include <cassert>

namespace
{
	// �\������e�L�X�g�̑S�̂̈ʒu
	constexpr int draw_text_pos_x = 350;
	constexpr int draw_text_pos_y = common::screen_height / 2 - 120;

	// �e�L�X�g�̕�����
	constexpr int text_space_y = 64;

	// �I������Ă��Ȃ��Ƃ��̕`��F
	constexpr int normal_color = 0x666666;

	// �I������Ă���Ƃ��̕`��F
	constexpr int choose_color = 0xffffff;
}

// �R���X�g���N�^
PadSettingScene::PadSettingScene(SceneManager& manager) : 
	SceneBase(manager),
	currentSelectItem_(0)
{
	// ���ڂ̕`��F��I������Ă��Ȃ��Ƃ��̐F�ɏ�����
	for (int i = 0; i < static_cast<int>(Item::TOTAL_VALUE); i++)
	{
		itemColorTable_.push_back(normal_color);
	}
}

// �f�X�g���N�^
PadSettingScene::~PadSettingScene()
{
	// �����Ȃ�
}

// �X�V
void PadSettingScene::Update()
{
	// �J���[�̏�����
	for (auto& item : itemColorTable_)
	{
		item = normal_color;
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
	itemColorTable_[currentSelectItem_] = choose_color;

	// �I������Ă��鍀�ڂ��ǂꂩ
	switch (static_cast<Item>(currentSelectItem_))
	{
	// �p�b�h�X�e�B�b�N�̉����x
	case Item::PAD_SENS_X:
		SaveData::GetInstance().SetPadStickSensitivityX();
		break;

	// �p�b�h�X�e�B�b�N�̏c���x
	case Item::PAD_SENS_Y:
		SaveData::GetInstance().SetPadStickSensitivityY();
		break;

	// �p�b�h�X�e�B�b�N�̉����]
	case Item::PAD_REVERSE_X:
		SaveData::GetInstance().SetPadStickReverseX();
		break;

	// �p�b�h�X�e�B�b�N�̏c���]
	case Item::PAD_REVERSE_Y:
		SaveData::GetInstance().SetPadStickReverseY();
		break;

	// �߂�
	case Item::BACK:
		if(InputState::IsTriggered(InputType::DECISION)) manager_.PopScene();
		return;

	// ���肦�Ȃ��̂Ŏ~�߂�
	default:
		assert(0);
	}

	// �߂�{�^���������ꂽ��
	if (InputState::IsTriggered(InputType::BACK))
	{
		// �ЂƂO�̃V�[���ɖ߂�
		manager_.PopScene();
		return;
	}
}

// �`��
void PadSettingScene::Draw()
{
	// �C���X�^���X�̎擾
	auto& stringManager = StringManager::GetInstance();
	auto& saveData = SaveData::GetInstance();

	// �^�C�g���̕\��
	stringManager.DrawStringCenter("PadSettingTitle", common::screen_width / 2, 100, 0xffffff);

	// ���ڂ̕\��
	int padSensXItem = static_cast<int>(Item::PAD_SENS_X);
	int padSensYItem = static_cast<int>(Item::PAD_SENS_Y);
	int padReverseXItem = static_cast<int>(Item::PAD_REVERSE_X);
	int padReverseYItem = static_cast<int>(Item::PAD_REVERSE_Y);
	int backItem = static_cast<int>(Item::BACK);
	stringManager.DrawStringCenter("PadSettingItemSensX", draw_text_pos_x, draw_text_pos_y + text_space_y * padSensXItem, itemColorTable_[padSensXItem]);
	stringManager.DrawStringCenter("PadSettingItemSensY", draw_text_pos_x, draw_text_pos_y + text_space_y * padSensYItem, itemColorTable_[padSensYItem]);
	stringManager.DrawStringCenter("PadSettingItemReverseX", draw_text_pos_x, draw_text_pos_y + text_space_y * padReverseXItem, itemColorTable_[padReverseXItem]);
	stringManager.DrawStringCenter("PadSettingItemReverseY", draw_text_pos_x, draw_text_pos_y + text_space_y * padReverseYItem, itemColorTable_[padReverseYItem]);
	stringManager.DrawStringCenter("PadSettingItemBack", common::screen_width / 2, draw_text_pos_y + text_space_y * backItem, itemColorTable_[backItem]);
	
	// �p�b�h�̊��x�̕\��
	stringManager.DrawFormatStringCenter("PadSettingNumber", saveData.GetSaveData().padStickSensitivityX, common::screen_width / 2, draw_text_pos_y + text_space_y * padSensXItem, itemColorTable_[padSensXItem]);
	stringManager.DrawFormatStringCenter("PadSettingNumber", saveData.GetSaveData().padStickSensitivityY, common::screen_width / 2, draw_text_pos_y + text_space_y * padSensYItem, itemColorTable_[padSensYItem]);

	// �p�b�h�̔��]�̕\��
	DrawPadReverse(saveData.GetSaveData().padStickReverseX, padReverseXItem);
	DrawPadReverse(saveData.GetSaveData().padStickReverseY, padReverseYItem);
}

// �p�b�h�̔��]�̃I���A�I�t�̕\��
void PadSettingScene::DrawPadReverse(bool isOn, int item)
{
	// �C���X�^���X�̎擾
	auto& stringManager = StringManager::GetInstance();

	// isOn�ɂ���ăI���A�I�t�\��
	(isOn) ? 
		// true : �I���\��
		(stringManager.DrawStringCenter("PadSettingItemReverseOn", common::screen_width / 2, draw_text_pos_y + text_space_y * item, itemColorTable_[item])) : 

		// false : �I�t�\��
		(stringManager.DrawStringCenter("PadSettingItemReverseOff", common::screen_width / 2, draw_text_pos_y + text_space_y * item, itemColorTable_[item]));
}
