#include <DxLib.h>
#include "Game.h"
#include "InputState.h"
#include "Util/SoundManager.h"
#include "Scene/SceneManager.h"
#include "Scene/TitleScene.h"
#include "Util/Effekseer3DEffectManager.h"
#include "EffekseerForDXLib.h"

// �v���O������ WinMain ����n�܂�܂�
int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_  HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd)
{
	// Window���[�h�ݒ�
	ChangeWindowMode(Game::window_mode);

	// Window���ݒ�
	SetMainWindowText(Game::title_text);

	// ��ʃT�C�Y�̐ݒ�
	SetGraphMode(Game::screen_width, Game::screen_height, Game::color_depth);

	// �Q�[�����ɃE�B���h�E���[�h��؂�ւ��Ă��O���t�B�b�N�n���h�������Z�b�g���Ȃ�
	SetChangeScreenModeGraphicsSystemResetFlag(false);

	// �ق��̃E�B���h�E��I�����Ă��Ă������悤�ɂ���
	SetAlwaysRunFlag(false);

	// �E�B���h�E�̃T�C�Y��ύX�\�ɂ���
	SetWindowSizeChangeEnableFlag(true);

	// DirectX11���g�p����悤�ɂ���B(DirectX9����)
	// Effekseer���g�p����ɂ͕K���ݒ肷��B
	SetUseDirect3DVersion(DX_DIRECT3D_11);

	// �t���X�N���[���E�C���h�E�̐؂�ւ��Ń��\�[�X��������̂�h��
	// Effekseer���g�p����ꍇ�͕K���ݒ肷��
	SetChangeScreenModeGraphicsSystemResetFlag(FALSE);

	// DX���C�u�����̃f�o�C�X���X�g�������̃R�[���o�b�N��ݒ肷��
	// �E�C���h�E�ƃt���X�N���[���̐؂�ւ�����������ꍇ�͕K�����s����
	// �������ADirectX11���g�p����ꍇ�͎��s����K�v�͂Ȃ�
	Effekseer_SetGraphicsDeviceLostCallbackFunctions();

	// XAudio ��L����
	SetEnableXAudioFlag(TRUE);

	// �P���[�g���ɑ�������l��ݒ肷��
	Set3DSoundOneMetre(Game::one_meter);

	auto& soundManager = SoundManager::GetInstance();
	auto& effectManager = Effekseer3DEffectManager::GetInstance();

	// �c�w���C�u��������������
	if (DxLib_Init() == -1)
	{
		// �G���[���N�����璼���ɏI��
		return -1;
	}
	// Effekseer�̏�����
	if (effectManager.Init() == -1)
	{
		// �G���[���N�����璼���ɏI��
		return -1;
	}
	
	// csv�t�@�C���ɉ����ăT�E���h�����[�h
	soundManager.LoadAndSaveSoundFileData();

	// �_�u���o�b�t�@���[�h
	// ����ʂɕ`��
	SetDrawScreen(DX_SCREEN_BACK);
	
	InputState input;
	SceneManager sceneManager;
	sceneManager.ChangeScene(new TitleScene(sceneManager));

	// �ُ킪�N�������ɏI��
	while (ProcessMessage() == 0)
	{
		LONGLONG time = GetNowHiPerformanceCount();

		// ��ʂ̃N���A
		ClearDrawScreen();

		input.Update();
		sceneManager.Update(input);
		effectManager.Update();

		sceneManager.Draw();
		effectManager.Draw();

		// FPS(Frame Per Second)�̎擾
		auto fps = GetFPS();

		// �`�施�ߐ��̎擾
		auto drawcall = GetDrawCallCount();

		DrawFormatString(10, 30, 0xffffff, "FPS = %2.2f", fps);
		DrawFormatString(10, 60, 0xffffff, "DC = %d", drawcall);

		// ����ʂ�\��ʂ����ւ���
		ScreenFlip();

		// esc�L�[����������I������
		if (CheckHitKey(KEY_INPUT_ESCAPE))	break;

		// fps��60�ɌŒ�
		while (GetNowHiPerformanceCount() - time < 16667)
		{

		}
	}

	effectManager.End();

	// �c�w���C�u�����g�p�̏I������
	DxLib_End();

	// �\�t�g�̏I�� 
	return 0;
}
