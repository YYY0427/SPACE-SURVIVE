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
	// �Z�[�u�f�[�^�̓ǂݍ���
	auto& saveData = SaveData::GetInstance();
	saveData.Load();

	// Window���[�h�ݒ�
	ChangeWindowMode(saveData.GetSaveData().windowMode);

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

	// 3D�ł̕`�揈���̃T�|�[�g
	SetUseLarge3DPositionSupport(TRUE);

	// �c�w���C�u��������������
	if (DxLib_Init() == -1)
	{
		// �G���[���N������~�߂�
		assert(0);
	}
	// �񓯊��ǂݍ��ݐݒ�ɕύX
//	SetUseASyncLoadFlag(TRUE);

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
//	MV1SetLoadModelPhysicsWorldGravity(-9.8f);
	
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

		// �X�V
		InputState::Update();
		sceneManager.Update();
		effectManager.Update();

		// �`��
		sceneManager.Draw();
		effectManager.Draw();

		// Window���[�h�ݒ�
		ChangeWindowMode(saveData.GetSaveData().windowMode);

#ifdef _DEBUG
		// FPS(Frame Per Second)�̎擾�ƕ`��
		auto fps = GetFPS();
		DrawFormatString(10, 30, 0xffffff, "FPS = %2.2f", fps);

		// �`�施�ߐ��̎擾�ƕ`��
		auto drawcall = GetDrawCallCount();
		DrawFormatString(10, 60, 0xffffff, "DC = %d", drawcall);
#endif
		// ����ʂ�\��ʂ����ւ���
		ScreenFlip();

		// �Q�[�����I�����邩�̃t���O�̃`�F�b�N
		// �t���O�������Ă����ꍇ���[�v�𔲂��ăQ�[�����I��
		if (sceneManager.GetIsGameEnd())	break;

#ifdef _DEBUG
		// esc�L�[����������I������
		if (CheckHitKey(KEY_INPUT_ESCAPE))	break;
#endif
		// fps���Œ�
		while (GetNowHiPerformanceCount() - time < (static_cast<long long>((1000 / common::fps)) * 1000))
		{

		}
	}

	// �I������
	soundManager.StopAllSound();
	stringManager.End();
	effectManager.End();
	DxLib_End();

	// �\�t�g�̏I�� 
	return 0;
}
