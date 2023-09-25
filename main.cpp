#include <DxLib.h>
#include "EffekseerForDXLib.h"
#include "common.h"
#include "Util/SoundManager.h"
#include "Scene/SceneManager.h"
#include "Scene/TitleScene.h"
#include "Scene/DebugScene.h"
#include "Util/Effekseer3DEffectManager.h"
#include "Util/SaveData.h"
#include "Util/InputState.h"
#include "Util/StringManager.h"

// �v���O������ WinMain ����n�܂�܂�
int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_  HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd)
{
	// Window���[�h�ݒ�
	ChangeWindowMode(common::window_mode);

	// Window���ݒ�
	SetMainWindowText(common::title_text);

	// ��ʃT�C�Y�̐ݒ�
	SetGraphMode(common::screen_width, common::screen_height, common::color_depth);

	// �Q�[�����ɃE�B���h�E���[�h��؂�ւ��Ă��O���t�B�b�N�n���h�������Z�b�g���Ȃ�
	SetChangeScreenModeGraphicsSystemResetFlag(TRUE);

	// �ق��̃E�B���h�E��I�����Ă��Ă������悤�ɂ���
	SetAlwaysRunFlag(TRUE);

	// �E�B���h�E�̃T�C�Y��ύX�\�ɂ���
	SetWindowSizeChangeEnableFlag(TRUE);

	// DirectX11���g�p
	SetUseDirect3DVersion(DX_DIRECT3D_11);
	
	// �P���[�g���ɑ�������l��ݒ肷��
	Set3DSoundOneMetre(common::one_meter);

	// XAudio��L����
	SetEnableXAudioFlag(TRUE);

	// ����������L����
	SetWaitVSyncFlag(TRUE);

	// �c�w���C�u��������������
	if (DxLib_Init() == -1)
	{
		// �G���[���N������~�߂�
		assert(0);
	}
	// Effekseer�̏�����
	auto& effectManager = Effekseer3DEffectManager::GetInstance();
	effectManager.Init();

	// �t���X�N���[���E�C���h�E�̐؂�ւ��Ń��\�[�X��������̂�h��
	// Effekseer���g�p����ꍇ�͕K���ݒ肷��
	SetChangeScreenModeGraphicsSystemResetFlag(FALSE);

	// DX���C�u�����̃f�o�C�X���X�g�������̃R�[���o�b�N��ݒ肷��
	// �E�C���h�E�ƃt���X�N���[���̐؂�ւ�����������ꍇ�͕K�����s����
	// �������ADirectX11���g�p����ꍇ�͎��s����K�v�͂Ȃ�
	Effekseer_SetGraphicsDeviceLostCallbackFunctions();
	
	// Z�o�b�t�@��L����
	SetUseZBufferFlag(TRUE);

	// Z�o�b�t�@�ւ̏������݂��s��
	SetWriteZBuffer3D(TRUE);

	// �|���S���̗��ʂ�`�悵�Ȃ�
	SetUseBackCulling(TRUE);

	// �d�͂̐ݒ�
	MV1SetLoadModelPhysicsWorldGravity(-9.8f);
	
	// �Z�[�u�f�[�^�̓ǂݍ���
	auto& saveData = SaveData::GetInstance();
	saveData.Load();

	// csv�t�@�C���ɉ����ăT�E���h�����[�h
	auto& soundManager = SoundManager::GetInstance();
	soundManager.LoadAndStoreSoundFileData();

	// csv�t�@�C���ɉ����ĕ���������[�h
	auto& stringManager = StringManager::GetInstance();
	stringManager.LoadAndStoreStringFileData();

	// ���̓^�C�v�̏�����
	InputState::Init();

	// �_�u���o�b�t�@���[�h
	// ����ʂɕ`��
	SetDrawScreen(DX_SCREEN_BACK);
	
	// �����V�[���̐ݒ�
	SceneManager sceneManager;
#ifdef _DEBUG
	sceneManager.ChangeScene(new DebugScene(sceneManager));
#else 
	sceneManager.ChangeScene(new TitleScene(sceneManager));
#endif

	// �ُ킪�N�������ɏI��
	while (ProcessMessage() == 0)
	{
		LONGLONG time = GetNowHiPerformanceCount();

		// ��ʂ̃N���A
		ClearDrawScreen();

		InputState::Update();
		sceneManager.Update();
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

	// �I������
	stringManager.End();
	effectManager.End();
	DxLib_End();

	// �\�t�g�̏I�� 
	return 0;
}
