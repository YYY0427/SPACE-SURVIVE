#pragma once

// TODO:�萔���܂Ƃ߂��Ƀt�@�C����������A����ƊO���t�@�C����
namespace common
{
	// Window���[�h�ݒ�
#ifdef _DEBUG
	constexpr bool window_mode = true;
#else
	constexpr bool window_mode = false;
#endif

	// Window��
	const char* const title_text = "SPACE SURVIVE";

	// Window�T�C�Y
	constexpr int screen_width = 1280;
	constexpr int screen_height = 720;

	// �J���[���[�h
	constexpr int color_depth = 32;		// 32 or 16

	// �Q�[�����ł�1m
	constexpr float one_meter = 100.0f;

	// �R���t�B�O�Őݒ肷�鉹�ʂ����ɕ����邩
	constexpr int config_volume_num = 5;

	// �R���t�B�O�Őݒ肷��p�b�h�̊��x�����ɕ����邩
	constexpr int config_pad_sensitivity_num = 10;

	// fps����
	constexpr int fps = 60;
};