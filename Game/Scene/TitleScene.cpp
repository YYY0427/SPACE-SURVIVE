#include <DxLib.h>
#include <cassert>
#include "TitleScene.h"
#include "SceneManager.h"
#include "MainScene.h"
#include "OptionScene.h"
#include "DebugScene.h"
#include "../Util/InputState.h"
#include "../Util/SoundManager.h"
#include "../Util/DrawFunctions.h"
#include "../Util/StringManager.h"
#include "../common.h"

namespace
{
	// �\������e�L�X�g�̑S�̂̈ʒu
	constexpr int draw_text_pos_y = common::screen_height / 2 + 100;

	// �e�L�X�g�̕�����
	constexpr int text_space_y = 70;
}

// �R���X�g���N�^
TitleScene::TitleScene(SceneManager& manager) :
	Scene(manager),
	updateFunc_(&TitleScene::NormalUpdate),
	currentSelectItem_(0)
{
	// �摜�̃��[�h
	handle_ = my::MyLoadGraph("Data/Image/title.png");

	// �t�F�[�h�̐ݒ�
	fadeDataTable_[static_cast<int>(Item::START)] = { 255, 8, true, true };
	fadeDataTable_[static_cast<int>(Item::END)] = { 255, 8, true, true };
	fadeDataTable_[static_cast<int>(Item::OPSITON)] = { 200, 8, true, true };

	// BGM��炷
	SoundManager::GetInstance().PlayBGM("bgmTest");
}

// �f�X�g���N�^
TitleScene::~TitleScene()
{
	DeleteGraph(handle_);
}

// �����o�֐��|�C���^�̍X�V
void TitleScene::Update()
{
	(this->*updateFunc_)();
}

// �ʏ�̍X�V
void TitleScene::NormalUpdate()
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

	// ���ւ̃{�^����������Ă���t�F�[�h�A�E�g�̊J�n
	if (InputState::IsTriggered(InputType::DECISION))
	{
		// �t�F�[�h�A�E�g�̊J�n
		StartFadeOut(fadeDataTable_[currentSelectItem_].fadeValue, fadeDataTable_[currentSelectItem_].fadeSpeed);
	}

	// �t�F�[�h�A�E�g���I��莟��V�[���J��
	if (IsStartFadeOutAfterFadingOut())
	{
		switch (static_cast<Item>(currentSelectItem_))
		{
		// �Q�[���X�^�[�g
		case Item::START:
			manager_.ChangeScene(new MainScene(manager_));
			return;

		// �I�v�V����
		case Item::OPSITON:
			manager_.PushScene(new OptionScene(manager_));
			break;

		// �Q�[���I��
		case Item::END:
			manager_.SetIsGameEnd(true);
			return;

		// ���肦�Ȃ��̂Ŏ~�߂�
		default:
			assert(0);
		}

		// PushScene�����ꍇ�V�[�����c�����܂܂Ȃ̂�
		// �t�F�[�h�C�����J�n����
		StartFadeIn();
		return;
	}

	// �t�F�[�h�̍X�V
	UpdateFade();
}

// �`��
void TitleScene::Draw()
{
	DrawGraph(0, 0, handle_, true);

	// ���݂̃V�[���̃e�L�X�g�`��
	DrawString(0, 0, "TitleScene", 0xffffff, true);

	// ���ڂ̕`��
	auto& stringManager = StringManager::GetInstance();
	stringManager.DrawStringCenter("TitleItemStart", common::screen_width / 2, draw_text_pos_y + text_space_y * static_cast<int>(Item::START), 0xffffff);
	stringManager.DrawStringCenter("TitleItemOption", common::screen_width / 2, draw_text_pos_y + text_space_y * static_cast<int>(Item::OPSITON), 0xffffff);
	stringManager.DrawStringCenter("TitleItemEnd", common::screen_width / 2, draw_text_pos_y + text_space_y * static_cast<int>(Item::END), 0xffffff);

	// �I�𒆂̍��ڂɃo�[��`��
	stringManager.DrawString("TitleItemSelectBarRight", common::screen_width / 2 - 100, draw_text_pos_y + text_space_y * currentSelectItem_, 0xffffff);
	stringManager.DrawString("TitleItemSelectBarLeft", common::screen_width / 2  + 90, draw_text_pos_y + text_space_y * currentSelectItem_, 0xffffff);

	// ���U�C�N�t�F�[�h�̕`��
	DrawGaussFade(fadeDataTable_[currentSelectItem_].isFade);

	// �t�F�[�h�̕`��
	DrawFade(fadeDataTable_[currentSelectItem_].isGaussFade);
}