#include "SaveData.h"
#include "SoundManager.h"
#include "Range.h"
#include "InputState.h"
#include "../common.h"
#include <string>
#include <cassert>
#include <fstream>
#include <DxLib.h>

namespace
{
	// �t�@�C���̃p�X
	const std::string data_file_path = "Data/Save/SaveData.dat";

	// �Z�[�u�f�[�^�o�[�W�����@
	constexpr int current_save_version = 0;
}

// �R���X�g���N�^
SaveData::SaveData() :
	soundManger_(SoundManager::GetInstance())
{
	
}

// �f�X�g���N�^
SaveData::~SaveData()
{
}

// �B��̃C���X�^���X��Ԃ�
SaveData& SaveData::GetInstance()
{
	// �B��̎���
	static SaveData instance;

	// �B��̎��ԎQ�Ƃ�Ԃ�
	return instance;
}

// �Z�[�u�f�[�^�̓ǂݍ���
void SaveData::Load()
{
	// �Z�[�u�f�[�^�̏�����
	InitData();

	std::ifstream ifs;
	ifs.open(data_file_path, std::ios_base::binary);

	// �t�@�C���ǂݍ��ݎ��s
	if (ifs.fail())
	{
		// �Z�[�u�f�[�^�����
		CreateNewData();
		return;
	}
	else
	{
		// �Z�[�u�f�[�^�̓ǂݍ���
		Data data;
		ifs.read(reinterpret_cast<char*>(&data), sizeof(Data));
		ifs.close();

		if (data.version == current_save_version)
		{
			// �ǂݍ��񂾃f�[�^�����ۂɎg�p����f�[�^�ɃR�s�[
			saveData_ = data;
		}
		else
		{
			// �����[�X��ɃZ�[�u�f�[�^�̃o�[�W�������ς���������
			assert(0);
		}
	}
}

// �Z�[�u�f�[�^�̏�������
void SaveData::Write()
{
	assert(saveData_.version == current_save_version);
	std::ofstream ofs(data_file_path, std::ios_base::binary);
	ofs.write(reinterpret_cast<char*>(&saveData_), sizeof(Data));
}

// �Z�[�u�f�[�^��V�K�쐬���ď㏑��
void SaveData::CreateNewData()
{
	// �Z�[�u�f�[�^�̏�����
	InitData();

	std::ofstream ofs(data_file_path, std::ios_base::binary);
	ofs.write(reinterpret_cast<char*>(&saveData_), sizeof(SaveData));
}

// �Z�[�u�f�[�^�̏�����
void SaveData::InitData()
{
	saveData_.version = current_save_version;
	saveData_.bgmVolume = 5;
	saveData_.seVolume = 5;
	saveData_.masterVolume = 5;
	saveData_.padStickSensitivityX = 6;
	saveData_.padStickSensitivityY = 3;
	saveData_.padStickReverseX = false;	
	saveData_.padStickReverseY = false;	
	saveData_.windowMode = false;
}

template<class T> void SaveData::SetConfigValue(T& configValue, int splitNum)
{
	if (InputState::IsTriggered(InputType::RIGHT))
	{
		configValue++;
	}
	if (InputState::IsTriggered(InputType::LEFT))
	{
		configValue--;
	}

	const Range<T> configValueRange(0, splitNum + 1);

	configValue = configValueRange.Wrap(configValue);

	// �Z�[�u�f�[�^�ɏ�������
	SaveData::GetInstance().Write();
}

// �S�̂̉��ʂ̐ݒ�
// �ő�l�𒴂����0�ɖ߂�
void SaveData::SetMasterVolume()
{
	SetConfigValue(saveData_.masterVolume, common::config_volume_num);

	if (InputState::IsTriggered(InputType::RIGHT) ||
		InputState::IsTriggered(InputType::LEFT))
	{
		soundManger_.SetVolume("bgmTest", 255);
	}
}

// BGM�̉��ʂ̐ݒ�
// �ő�l�𒴂����0�ɖ߂�
void SaveData::SetBgmVolume()
{
	SetConfigValue(saveData_.bgmVolume, common::config_volume_num);

	if (InputState::IsTriggered(InputType::RIGHT) ||
		InputState::IsTriggered(InputType::LEFT))
	{
		soundManger_.SetVolume("bgmTest", 255);
	}
}

// SE�̉��ʂ̐ݒ�
// �ő�l�𒴂����0�ɖ߂�
void SaveData::SetSeVolume()
{
	SetConfigValue(saveData_.seVolume, common::config_volume_num);

	if (InputState::IsTriggered(InputType::RIGHT) ||
		InputState::IsTriggered(InputType::LEFT))
	{
		soundManger_.Play("cursorTest");
	}
}

// �p�b�h�̃X�e�B�b�N��X�����x�̐ݒ�
// �ő�l�𒴂����0�ɖ߂�
void SaveData::SetPadStickSensitivityX()
{
	SetConfigValue(saveData_.padStickSensitivityX, common::config_pad_sensitivity_num);
}

// �p�b�h�̃X�e�B�b�N��Y�����x�̐ݒ�
// �ő�l�𒴂����0�ɖ߂�
void SaveData::SetPadStickSensitivityY()
{
	SetConfigValue(saveData_.padStickSensitivityY, common::config_pad_sensitivity_num);
}

// �p�b�h�̃X�e�B�b�N��X�����o�[�X�̐ݒ�
void SaveData::SetPadStickReverseX()
{
	SetConfigSwitch(saveData_.padStickReverseX);
}

// �p�b�h�̃X�e�B�b�N��Y�����o�[�X�̐ݒ�
void SaveData::SetPadStickReverseY()
{
	SetConfigSwitch(saveData_.padStickReverseY);
}

void SaveData::SetWindowMode()
{
	SetConfigSwitch(saveData_.windowMode);
}

SaveData::Data SaveData::GetSaveData() const
{
	return saveData_;
}

void SaveData::SetConfigSwitch(bool& config)
{
	if (InputState::IsTriggered(InputType::RIGHT) ||
		InputState::IsTriggered(InputType::LEFT))
	{
		if (config)
		{
			config = false;
		}
		else
		{
			config = true;
		}
	}

	// �Z�[�u�f�[�^�ɏ�������
	SaveData::GetInstance().Write();
}
