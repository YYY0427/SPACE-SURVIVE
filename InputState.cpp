#include "InputState.h"
#include <DxLib.h>

InputState::InputState()
{
	defaultMapTable_[InputType::next] = { {InputCategory::keybd, KEY_INPUT_RETURN},
										{InputCategory::pad, PAD_INPUT_R },			//�X�^�[�g�{�^��
										{InputCategory::mouse, MOUSE_INPUT_LEFT } };

	defaultMapTable_[InputType::prev] = { {InputCategory::keybd, KEY_INPUT_SPACE},
										{InputCategory::pad, PAD_INPUT_7} };		//�o�b�N�{�^��

	defaultMapTable_[InputType::pause] = { {InputCategory::keybd, KEY_INPUT_P},
										{InputCategory::pad, PAD_INPUT_L } };		//�Z���N�g�{�^��


	defaultMapTable_[InputType::keyconf] = { {InputCategory::keybd, KEY_INPUT_K},
										{InputCategory::pad, PAD_INPUT_Y } };		//���V�����_�[

	defaultMapTable_[InputType::change] = { {InputCategory::keybd, KEY_INPUT_C},
										{InputCategory::pad, PAD_INPUT_Z } };		//�E�V�����_�[

	defaultMapTable_[InputType::up] = { {InputCategory::keybd, KEY_INPUT_UP},
										{InputCategory::pad, PAD_INPUT_UP } };		//��

	defaultMapTable_[InputType::down] = { {InputCategory::keybd, KEY_INPUT_DOWN},
										{InputCategory::pad, PAD_INPUT_DOWN } };	//��

	defaultMapTable_[InputType::right] = { {InputCategory::keybd, KEY_INPUT_RIGHT},
										{InputCategory::pad, PAD_INPUT_RIGHT } };	//��

	defaultMapTable_[InputType::left] = { {InputCategory::keybd, KEY_INPUT_LEFT},
										{InputCategory::pad, PAD_INPUT_LEFT } };	//��

	defaultMapTable_[InputType::shot] = { {InputCategory::keybd, KEY_INPUT_Z},
										{InputCategory::pad, PAD_INPUT_C } };		//�V���b�g

	defaultMapTable_[InputType::rapid] = { {InputCategory::keybd, KEY_INPUT_A},
									{InputCategory::pad, PAD_INPUT_A } };			//�A��

	defaultMapTable_[InputType::switcing] = { {InputCategory::keybd, KEY_INPUT_X},
									{InputCategory::pad, PAD_INPUT_X } };			//


	inputMapTable_ = defaultMapTable_;

	LoadKeyInfo();

	//�ꎞ�}�b�v�e�[�u���ɃR�s�[
	tempMapTable_ = inputMapTable_;

	//���̓^�C�v�̖��O�̃e�[�u�������
	inputNameTable_[InputType::next] = "next";
	inputNameTable_[InputType::prev] = "prev";
	inputNameTable_[InputType::pause] = "pause";
	inputNameTable_[InputType::keyconf] = "keyconf";
	inputNameTable_[InputType::change] = "change";
	inputNameTable_[InputType::shot] = "shot";
	inputNameTable_[InputType::rapid] = "rapid";
	inputNameTable_[InputType::switcing] = "switcihg";

	currentInput_.resize(static_cast<int>(InputType::max));
	lastInput_.resize(static_cast<int>(InputType::max));
}

void InputState::Update()
{
	lastInput_ = currentInput_;	//���O�̓��͏����L�����Ă���

	char keystate[256];
	GetHitKeyStateAll(keystate); //�S�L�[���擾

	int padState = GetJoypadInputState(DX_INPUT_PAD1);	//�p�b�h1�R���̏����擾����
	int mouseState = GetMouseInput();

	//�}�b�v�̑S�������[�v����
	for (const auto& keymap : inputMapTable_)
	{
		//���͏��z������[�v����
		for (const auto& input : keymap.second)
		{
			//����input�̒��g�́Akeybd,KEY_INPUT_RETURN�Ȃǂ̃Z�b�g(InputInfo)�������Ă���
			//keymap.second�́A���̓��͏��Z�b�gInputInfo�������Ă���
			//keymap.first�́A�Ή�����Q�[�����͖���"InputType::next"�Ȃǂ������Ă���
			if (input.cat == InputCategory::keybd)
			{
				currentInput_[static_cast<int>(keymap.first)] = keystate[input.id];
			}
			else if (input.cat == InputCategory::pad)
			{
				currentInput_[static_cast<int>(keymap.first)] = padState & input.id;
			}
			else if (input.cat == InputCategory::mouse)
			{
				currentInput_[static_cast<int>(keymap.first)] = mouseState & input.id;
			}
			//3�̓��͂̂����ǂꂩ��true������������u���͂���Ă�v
			//�Ƃ݂Ȃ��āAbreak����B
			if (currentInput_[static_cast<int>(keymap.first)])
			{
				break;
			}
		}
	}
}

void InputState::RewriteInputInfo(InputType type, InputCategory cat, int id)
{
	//���͎��(���蓖�Đ�)���Ȃ���΁A�����Ȃ̂Ŗ������܂�
	if (tempMapTable_.count(type) == 0)
	{
		return;
	}
	bool isRewrite = false;
	for (auto& inputInfo : tempMapTable_[type])
	{
		//�J�e�S�����q�b�g������
		if (inputInfo.cat == cat)
		{
			//ID���㏑������
			inputInfo.id = id;
			isRewrite = true;
			break;
		}
	}
	//�����J�e�S�������݂��Ȃ�������A�����Œǉ����Ă���
	if (!isRewrite)
	{
		tempMapTable_[type].push_back({ cat, id });
	}
}

void InputState::CommitChangedInputInfo()
{
	inputMapTable_ = tempMapTable_;
}

void InputState::RollbackChangedInputInfo()
{
	tempMapTable_ = inputMapTable_;
}

void InputState::ResetInputInfo()
{
	inputMapTable_ = defaultMapTable_;
	tempMapTable_ = defaultMapTable_;
}

void InputState::SaveKeyInfo() const
{
	FILE* fp = nullptr;

	auto err = fopen_s(&fp, "key.info", "wb");
	if (fp == nullptr)
	{
		return;
	}

	//���z�L�[�^�C�v�̐�����������
	int keytypeNum = static_cast<int>(inputMapTable_.size());
	fwrite(&keytypeNum, sizeof(keytypeNum), 1, fp);

	//���z�L�[�^�C�v(next, prev�Ȃ�)�̃��[�v
	for (const auto& key : inputMapTable_)
	{
		int keytype = static_cast<int>(key.first);

		//���z�L�[�ԍ�
		fwrite(&keytype, sizeof(keytype), 1, fp);
		int dataSize = static_cast<int>(key.second.size());

		//���������̓f�[�^������̂�
		fwrite(&dataSize, sizeof(dataSize), 1, fp);

		//vector�^��data()�͐擪�̃A�h���X��Ԃ�
		//��C�ɂ��̓��͂̃f�[�^����������
		fwrite(key.second.data(), dataSize * sizeof(InputInfo), 1, fp);
	}
	fclose(fp);
}

void InputState::LoadKeyInfo()
{
	int handle = FileRead_open("key.info");

	if (handle == 0)
	{
		return;
	}

	int keyTypeNum = 0;

	FileRead_read(&keyTypeNum, sizeof(keyTypeNum), handle);
	inputMapTable_.clear();

	for (int i = 0; i < keyTypeNum; i++)
	{
		int  inputType;
		FileRead_read(&inputType, sizeof(inputType), handle);

		int dataSize = 0;
		FileRead_read(&dataSize, sizeof(dataSize), handle);
		std::vector<InputInfo> inputInfoes(dataSize);
		FileRead_read(inputInfoes.data(), sizeof(InputInfo) * dataSize, handle);
		inputMapTable_[static_cast<InputType>(inputType)] = inputInfoes;
	}
	tempMapTable_ = inputMapTable_;
	FileRead_close(handle);
}

bool InputState::IsPressed(InputType type) const
{
	return currentInput_[static_cast<int>(type)];
}

bool InputState::IsTriggered(InputType type) const
{
	return IsPressed(type) && !lastInput_[static_cast<int>(type)];
}
