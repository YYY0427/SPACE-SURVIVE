#include "InputState.h"
#include <DxLib.h>

/// <summary>
/// �R���X�g���N�^
/// </summary>
InputState::InputState()
{
	// �{�^���̓��̓^�C�v���ǂ̃{�^���ɂ��邩�̐ݒ�
	inputMapTable_[InputType::DECISION] = { {InputCategory::PAD,  XINPUT_BUTTON_A } };			// A�{�^��
	inputMapTable_[InputType::BACK] = { {InputCategory::PAD, XINPUT_BUTTON_B} };				// B�{�^��
	inputMapTable_[InputType::PAUSE] = { {InputCategory::PAD, XINPUT_BUTTON_START } };			// �X�^�[�g�{�^��
	inputMapTable_[InputType::UP] = { {InputCategory::PAD, XINPUT_BUTTON_DPAD_UP } };			// ��
	inputMapTable_[InputType::DOWN] = { {InputCategory::PAD, XINPUT_BUTTON_DPAD_DOWN } };		// ��
	inputMapTable_[InputType::RIGHT] = { {InputCategory::PAD, XINPUT_BUTTON_DPAD_RIGHT } };		// ��
	inputMapTable_[InputType::LEFT] = { {InputCategory::PAD, XINPUT_BUTTON_DPAD_LEFT } };		// ��

	// �ݒ肵���{�^���̐��ɂ���Ĕz��̐���ύX
	currentInput_.resize(static_cast<int>(InputType::NUM));
	lastInput_.resize(static_cast<int>(InputType::NUM));
}

/// <summary>
/// ���͏��̍X�V
/// (����)���t���[��Update���Ă΂Ȃ��Ɠ��͏�Ԃ͍X�V����Ȃ�
/// </summary>
void InputState::Update()
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

/// <summary>
/// �{�^���������ꂽ�u�Ԃ̓��͏��̎擾
/// </summary>
/// <param name="type">�擾���������̓^�C�v</param>
/// <returns>true : ���́Afalse : �����</returns>
bool InputState::IsPressed(InputType type) const
{
	return currentInput_[static_cast<int>(type)];
}

/// <summary>
/// �{�^����������Ă���Ԃ̓��͏��̎擾
/// </summary>
/// <param name="type">�擾���������̓^�C�v</param>
/// <returns>true : ���͒��Afalse : �����</returns>
bool InputState::IsTriggered(InputType type) const
{
	return IsPressed(type) && !lastInput_[static_cast<int>(type)];
}

/// <summary>
/// �p�b�h�̃X�e�B�b�N�̓��͏����擾
/// </summary>
/// <param name="stickType">�X�e�B�b�N�̉E������</param>
/// <returns>�X�e�B�b�N�̂ǂ̓��̓^�C�v��</returns>
XInputTypeStick InputState::IsXInputStick(XInputType stickType) const
{
	// �p�b�h�̏��̎擾
	XINPUT_STATE  padState;
	GetJoypadXInputState(DX_INPUT_PAD1, &padState);

	// �E�X�e�B�b�N�����X�e�B�b�N��
	float x, y;
	if (stickType == XInputType::LEFT)
	{
		// padState����擾�����l��-1.0~1.0�ɕϊ�
		x = static_cast<float>(padState.ThumbLX / 32767.0f);
		y = static_cast<float>(padState.ThumbLY / 32767.0f);
	}
	else
	{
		// padState����擾�����l��-1.0~1.0�ɕϊ�
		x = static_cast<float>(padState.ThumbRX / 32767.0f);
		y = static_cast<float>(padState.ThumbRY / 32767.0f);
	}

	// �ϊ������l�̊����ɂ����return����
	if (x < 0.8f && x > 0.3f)
	{
		// �E�ɂ�����Ɠ���
		return XInputTypeStick::LITTLE_RIGHT;
	}
	else if (x > 0.9f)
	{
		// �E�ɑ傫������
		return XInputTypeStick::RIGHT;
	}
	else if (x < -0.3f && x > -0.8f)
	{
		// ���ɏ�������
		return XInputTypeStick::LITTLE_LEFT;
	}
	else if (x < -0.9f)
	{
		// ���ɑ傫������
		return XInputTypeStick::LEFT;
	}

	if (y < 0.8f && y > 0.3f)
	{
		// ��ɂ�����Ɠ���
		return XInputTypeStick::LITTLE_UP;
	}
	else if (y > 0.9f)
	{
		// ��ɑ傫������
		return XInputTypeStick::UP;
	}
	else if (y < -0.3f && y > -0.8f)
	{
		// ���ɏ�������
		return XInputTypeStick::LITTLE_DOWN;
	}
	else if (y < -0.9f)
	{
		// ���ɑ傫������
		return XInputTypeStick::DOWN;
	}
	// �����ĂȂ�
	return XInputTypeStick::NONE;
}

/// <summary>
/// �p�b�h�̃g���K�[�̓��͏��̎擾
/// </summary>
/// <param name="type">�擾���������̓^�C�v</param>
/// <returns>true : ���͒��Afalse : �����</returns>
bool InputState::IsXInputTrigger(XInputType type) const
{
	// �p�b�h�̏��̎擾
	XINPUT_STATE  padState;
	GetJoypadXInputState(DX_INPUT_PAD1, &padState);

	// �g���K�[�̍����E��
	if (type == XInputType::LEFT)
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