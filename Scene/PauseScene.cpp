#include "PauseScene.h"
#include "SceneManager.h"
#include "OptionScene.h"
#include "TitleScene.h"
#include "../Util/InputState.h"
#include "../Util/SoundManager.h"
#include "../Util/StringManager.h"
#include "../common.h"
#include <DxLib.h>

namespace
{
	// �\������e�L�X�g�̑S�̂̈ʒu
	constexpr int draw_text_pos_x = common::screen_width / 2;
	constexpr int draw_text_pos_y = common::screen_height / 2 - 100;

	// �e�L�X�g�̕�����
	constexpr int text_space_y = 64;
}

// �R���X�g���N�^
PauseScene::PauseScene(SceneManager& manager) :
	Scene(manager),
	currentSelectItem_(0)
{
	// �t�F�[�h�C�����s��Ȃ�
	SetFadeBright(0);

	// �e���ڂɂ���ăt�F�[�h�̐ݒ�
	fadeConfigTable_[static_cast<int>(Item::CONTINUE)] = { 0, 8, false, false, false};
	fadeConfigTable_[static_cast<int>(Item::OPTION)] = { 255, 16, false , false, true };
	fadeConfigTable_[static_cast<int>(Item::TITLE)] = { 255, 8, true, true, false };
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
	if (InputState::IsTriggered(InputType::UP))
	{
		currentSelectItem_ = ((currentSelectItem_ - 1) + static_cast<int>(Item::TOTAL_VALUE)) % static_cast<int>(Item::TOTAL_VALUE);
	}
	else if (InputState::IsTriggered(InputType::DOWN))
	{
		currentSelectItem_ = (currentSelectItem_ + 1) % static_cast<int>(Item::TOTAL_VALUE);
	}

	// ����{�^������������t�F�[�h�A�E�g�J�n
	if (InputState::IsTriggered(InputType::DECISION))
	{
		// �I�����ɂ���ăt�F�[�h�̒l��ݒ�
		// �t�F�[�h�A�E�g�J�n
		StartFadeOut(fadeConfigTable_[currentSelectItem_].fadeValue, fadeConfigTable_[currentSelectItem_].fadeSpeed);
	}

	// �|�[�Y��ʂ���߂�
	if (InputState::IsTriggered(InputType::BACK) || InputState::IsTriggered(InputType::PAUSE))
	{
		manager_.PopScene();
		return;
	}

	// �t�F�[�h�A�E�g���I��肵�����I�����ꂽ���ڂɔ��
	if (IsStartFadeOutAfterFadingOut())
	{
		switch (currentSelectItem_)
		{
		// ������
		case static_cast<int>(Item::CONTINUE):

			manager_.PopScene();
			return;
		// �I�v�V����
		case static_cast<int>(Item::OPTION):

			manager_.PushScene(new OptionScene(manager_));
			break;
		// �^�C�g��
		case static_cast<int>(Item::TITLE):

			// �S�ẴT�E���h���~�߂�
			SoundManager::GetInstance().StopAllSound();

			manager_.PopAllSceneAndChangeScene(new TitleScene(manager_));
			return;
		}
		// PushScene�����ꍇ�V�[�����c�����܂܂Ȃ̂�
		// �t�F�[�h�C���̐ݒ�
		StartFadeIn();
		return;
	}

	// �t�F�[�h�̍X�V
	UpdateFade();
}

// �`��
void PauseScene::Draw()
{
	// �C���X�^���X�̎擾
	auto& stringManager = StringManager::GetInstance();

	// ���ڂɐݒ肳�ꂽ�t�F�[�h�ݒ�ɊY������Ƃ������������t�F�[�h����
	if (fadeConfigTable_[currentSelectItem_].isOnlyStringFade)
	{
		// �`�擧���x�̐ݒ�
		int fade = 255 - GetFadeBright();
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, fade);
	}

	// ���ڂ̕\��
	stringManager.DrawStringCenter("PauseTitle", draw_text_pos_x, 100, 0xffffff);
	stringManager.DrawStringCenter("PauseItemContinue", draw_text_pos_x, draw_text_pos_y + text_space_y * static_cast<int>(Item::CONTINUE), 0xffffff);
	stringManager.DrawStringCenter("PauseItemOption", draw_text_pos_x, draw_text_pos_y + text_space_y * static_cast<int>(Item::OPTION), 0xffffff);
	stringManager.DrawStringCenter("PauseItemTitle", draw_text_pos_x, draw_text_pos_y + text_space_y * static_cast<int>(Item::TITLE), 0xffffff);

	// �I������Ă��鍀�ڂɃo�[��`��
	stringManager.DrawStringCenter("PausetemSelectBarRight", draw_text_pos_x - 100, draw_text_pos_y + text_space_y * currentSelectItem_, 0xffffff);
	stringManager.DrawStringCenter("PausetemSelectBarLeft", draw_text_pos_x + 100, draw_text_pos_y + text_space_y * currentSelectItem_, 0xffffff);

	// �`��̐ݒ�̏�����
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	// �t�F�[�h�̕`�� 
	// ���ڂɐݒ肳�ꂽ�t�F�[�h�ݒ�ɊY������Ƃ������t�F�[�h����
	DrawFade(fadeConfigTable_[currentSelectItem_].isFade);

	// ���U�C�N�t�F�[�h�̕`��
	// ���ڂɐݒ肳�ꂽ�t�F�[�h�ݒ�ɊY������Ƃ������t�F�[�h����
	DrawGaussFade(fadeConfigTable_[currentSelectItem_].isGaussFade);
}