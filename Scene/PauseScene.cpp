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
}

/// <summary>
/// �R���X�g���N�^
/// </summary>
/// <param name="manager">�V�[���}�l�[�W���[�̎Q��</param>
PauseScene::PauseScene(SceneManager& manager) :
	Scene(manager)
{
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
PauseScene::~PauseScene()
{
}

/// <summary>
/// �X�V
/// </summary>
void PauseScene::Update(const InputState& input)
{
	if (input.IsTriggered(InputType::BACK))
	{
		manager_.PopScene();
		return;
	}
}

/// <summary>
/// �`��
/// </summary>
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
}