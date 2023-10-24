#include "InputState.h"
#include <DxLib.h>
#include <map>
#include <vector>
#include <string>
#include <cmath>

namespace
{
	// ���z���͏��Ǝ��ۂ̓��͂̃e�[�u��
	std::map<InputType, std::vector<InputInfo>> inputMapTable_;

	// ���݂̓��͏��(�����Ă邩�����ĂȂ���)
	std::vector<bool> currentInput_;

	// ���O�̓��͏��(���O�����Ă邩�����ĂȂ���)
	std::vector<bool> lastInput_;
}

namespace InputState
{
	// ������
	void Init()
	{
		// �{�^���̓��̓^�C�v���ǂ̃{�^���ɂ��邩�̐ݒ�
		inputMapTable_[InputType::DECISION] = { {InputCategory::PAD,  XINPUT_BUTTON_A } };			// A�{�^��
		inputMapTable_[InputType::BACK] = { {InputCategory::PAD, XINPUT_BUTTON_B} };				// B�{�^��
		inputMapTable_[InputType::PAUSE] = { {InputCategory::PAD, XINPUT_BUTTON_START } };			// �X�^�[�g�{�^��
		inputMapTable_[InputType::SLOW] = { {InputCategory::PAD, XINPUT_BUTTON_B } };				// �X���[
		inputMapTable_[InputType::BOOST] = { {InputCategory::PAD, XINPUT_BUTTON_A } };				// �u�[�X�g
		inputMapTable_[InputType::CAMERA_INVERSION] = { {InputCategory::PAD, XINPUT_BUTTON_RIGHT_SHOULDER } };	// �J�������]
		inputMapTable_[InputType::UP] = { {InputCategory::PAD, XINPUT_BUTTON_DPAD_UP } };			// ��
		inputMapTable_[InputType::DOWN] = { {InputCategory::PAD, XINPUT_BUTTON_DPAD_DOWN } };		// ��
		inputMapTable_[InputType::RIGHT] = { {InputCategory::PAD, XINPUT_BUTTON_DPAD_RIGHT } };		// ��
		inputMapTable_[InputType::LEFT] = { {InputCategory::PAD, XINPUT_BUTTON_DPAD_LEFT } };		// ��

		// �ݒ肵���{�^���̐��ɂ���Ĕz��̐���ύX
		currentInput_.resize(static_cast<int>(InputType::TOTAL_VALUE));
		lastInput_.resize(static_cast<int>(InputType::TOTAL_VALUE));
	}

	// ���͏��̍X�V
	void Update()
	{
		// ���O�̓��͏����L�����Ă���
		lastInput_ = currentInput_;

		// �p�b�h�̏��̎擾
		XINPUT_STATE  padState;
		GetJoypadXInputState(DX_INPUT_PAD1, &padState);

		// �L�[���̎擾
		char keystate[256];
		GetHitKeyStateAll(keystate);

		// �}�E�X���̎擾
		int mouseState = GetMouseInput();

		// �}�b�v�̑S�������[�v
		for (const auto& keymap : inputMapTable_)
		{
			// ���͏��z������[�v
			for (const auto& input : keymap.second)
			{
				// ���͏�񂩂�ǂ̃{�^�������͂���Ă��邩�m�F
				if (input.cat == InputCategory::KEYBORD)
				{
					currentInput_[static_cast<int>(keymap.first)] = keystate[input.id];
				}
				else if (input.cat == InputCategory::PAD)
				{
					currentInput_[static_cast<int>(keymap.first)] = padState.Buttons[input.id];
				}
				else if (input.cat == InputCategory::MOUSE)
				{
					currentInput_[static_cast<int>(keymap.first)] = mouseState & input.id;
				}
				// 3�̓��͂̂����ǂꂩ��true����������͂���Ă邩��break
				if (currentInput_[static_cast<int>(keymap.first)])
				{
					break;
				}
			}
		}
	}

	// �{�^����������Ă���Ԃ̓��͏��̎擾
	bool IsPressed(InputType type)
	{
		return currentInput_[static_cast<int>(type)];
	}

	// �{�^���������ꂽ�u�Ԃ̓��͏��̎擾
	bool IsTriggered(InputType type)
	{
		return IsPressed(type) && !lastInput_[static_cast<int>(type)];
	}

	// �p�b�h�̃X�e�B�b�N�̓��͏����擾
	int IsPadStick(PadLR stick, PadStickInputType type)
	{
		// �p�b�h�̏��̎擾
		XINPUT_STATE  padState;
		GetJoypadXInputState(DX_INPUT_PAD1, &padState);

		// �E�X�e�B�b�N�����X�e�B�b�N��
		float x, y;
		if (stick == PadLR::LEFT)
		{
			// padState����擾�����l��-10.0~10.0�ɕϊ�
			x = static_cast<float>(padState.ThumbLX / 32767.0f * 10);
			y = static_cast<float>(padState.ThumbLY / 32767.0f * 10);
		}
		else
		{
			// padState����擾�����l��-10.0~10.0�ɕϊ�
			x = static_cast<float>(padState.ThumbRX / 32767.0f * 10);
			y = static_cast<float>(padState.ThumbRY / 32767.0f * 10);
		}
		if (type == PadStickInputType::LEFT && x < -0.1f)
		{
			return abs(static_cast<int>(x));
		}
		if (type == PadStickInputType::RIGHT && x > 0.1f)
		{
			return abs(static_cast<int>(x));
		}
		if (type == PadStickInputType::UP && y < -0.1f)
		{
			return abs(static_cast<int>(y));
		}
		if (type == PadStickInputType::DOWN && y > 0.1f)
		{
			return abs(static_cast<int>(y));
		}
		// ���͂Ȃ�
		return 0;
	}

	// �p�b�h�̃g���K�[�̓��͏��̎擾
	bool IsPadTrigger(PadLR type)
	{
		// �p�b�h�̏��̎擾
		XINPUT_STATE  padState;
		GetJoypadXInputState(DX_INPUT_PAD1, &padState);

		// �g���K�[�̍����E��
		if (type == PadLR::LEFT)
		{
			// �g���K�[�������ȏ㉟����Ă�����true
			return 	padState.LeftTrigger > padState.LeftTrigger / 2;
		}
		else
		{
			// �g���K�[�������ȏ㉟����Ă�����true
			return 	padState.RightTrigger > padState.RightTrigger / 2;
		}
	}
}