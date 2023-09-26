#include <DxLib.h>
#include "TitleScene.h"
#include "SceneManager.h"
#include "MainScene.h"
#include "ConfigScene.h"
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
	constexpr int text_space = 70;
}

// �R���X�g���N�^
TitleScene::TitleScene(SceneManager& manager) :
	Scene(manager),
	updateFunc_(&TitleScene::NormalUpdate),
	currentSelectItem_(0)
{
}

// �f�X�g���N�^
TitleScene::~TitleScene()
{
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
	// �t�F�[�h���̏ꍇ�͏������s��Ȃ�
	int sceneItemTotalValue = static_cast<int>(Item::TOTAL_VALUE);
	if (InputState::IsTriggered(InputType::UP) && !IsFadeing())
	{
		currentSelectItem_ = ((currentSelectItem_ - 1) + sceneItemTotalValue) % sceneItemTotalValue;
	}
	else if (InputState::IsTriggered(InputType::DOWN) && !IsFadeing())
	{
		currentSelectItem_ = (currentSelectItem_ + 1) % sceneItemTotalValue;
	}

	// ���ւ̃{�^����������āA�t�F�[�h���łȂ�������t�F�[�h�A�E�g�̊J�n
	if (InputState::IsTriggered(InputType::DECISION) && !IsFadeing())
	{
		// �t�F�[�h�A�E�g�̊J�n
		StartFadeOut();

		// �t�F�[�h�A�E�g���s��ꂽ���ǂ����̃t���O�𗧂Ă�
		// �V�[���J�ڂ̍ہA�t�F�[�h�A�E�g���s��ꂽ���ǂ������m�F���邽��
		isFadeOut_ = true;
	}
	// �t�F�[�h�A�E�g���I��莟��V�[���J��
	if (isFadeOut_ && !IsFadingOut())
	{
		if (currentSelectItem_ == static_cast<int>(Item::START))
		{
			isFadeOut_ = false;
			manager_.ChangeScene(new MainScene(manager_));
			return;
		}
		else if (currentSelectItem_ == static_cast<int>(Item::OPSITON))
		{
			isFadeOut_ = false;
			fadeSpeed_ = -8;
			manager_.PushScene(new ConfigScene(manager_));
			return;
		}
		else if (currentSelectItem_ == static_cast<int>(Item::END))
		{
			manager_.SetIsGameEnd(true);
			return;
		}
	}
	// �t�F�[�h�̍X�V
	UpdateFade();
}

// �`��
void TitleScene::Draw()
{
	// ���݂̃V�[���̃e�L�X�g�\��
	DrawString(0, 0, "TitleScene", 0xffffff, true);

	auto& stringManager = StringManager::GetInstance();
	stringManager.DrawStringCenter("TitleItemStart", draw_text_pos_y + text_space * static_cast<int>(Item::START), 0xffffff);
	stringManager.DrawStringCenter("TitleItemOption", draw_text_pos_y + text_space * static_cast<int>(Item::OPSITON), 0xffffff);
	stringManager.DrawStringCenter("TitleItemEnd", draw_text_pos_y + text_space * static_cast<int>(Item::END), 0xffffff);

	// ���ݑI�𒆂̍��ڂ̉��Ɂ���\��
	DrawString(200, draw_text_pos_y + text_space * currentSelectItem_, "��", 0xff0000);

	// �t�F�[�h�̕`��
	DrawGaussFade();

	// �t�F�[�h�̕`��
	DrawFade();
}