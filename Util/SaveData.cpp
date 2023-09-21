#include "SaveData.h"
#include "../common.h"
#include <string>
#include <cassert>
#include <fstream>
#include <DxLib.h>

namespace
{
	// �Z�[�u�f�[�^�t�@�C����
	const std::string data_file_path = "Data/Save/Data.dat";

	// �Z�[�u�f�[�^�o�[�W�����@
	constexpr int current_save_version = 0;
}

// �R���X�g���N�^
SaveData::SaveData()
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
			data_ = data;
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
	assert(data_.version == current_save_version);
	std::ofstream ofs(data_file_path, std::ios_base::binary);
	ofs.write(reinterpret_cast<char*>(&data_), sizeof(Data));
}

// �Z�[�u�f�[�^��V�K�쐬���ď㏑��
void SaveData::CreateNewData()
{
	// �Z�[�u�f�[�^�̏�����
	InitData();

	std::ofstream ofs(data_file_path, std::ios_base::binary);
	ofs.write(reinterpret_cast<char*>(&data_), sizeof(SaveData));
}

// �Z�[�u�f�[�^�̏�����
void SaveData::InitData()
{
	data_.version = current_save_version;
	data_.volumeBgm = 3;
	data_.volumeSe = 3;
	data_.padStickSensitivityX = 5;
	data_.padStickSensitivityY = 3;
	data_.padStickReverseX = false;	
	data_.padStickReverseY = false;	
}

// BGM�̉��ʂ̎擾
int SaveData::GetBgmVolume() const
{
	return data_.volumeBgm;
}

// SE�̉��ʂ̎擾
int SaveData::GetSeVolume() const
{
	return data_.volumeSe;
}

// �p�b�h�̃X�e�B�b�N��X�����x�̎擾
int SaveData::GetPadStickSensitivityX() const
{
	return data_.padStickSensitivityX;
}

// �p�b�h�̃X�e�B�b�N��Y�����x�̎擾
int SaveData::GetPadStickSensitivityY() const
{
	return data_.padStickSensitivityY;
}

// �p�b�h�̃X�e�B�b�N��X�����o�[�X�̎擾
bool SaveData::GetPadStickReverseX() const
{
	return data_.padStickReverseX;
}

// �p�b�h�̃X�e�B�b�N��Y�����o�[�X�̎擾
bool SaveData::GetPadStickReverseY() const
{
	return data_.padStickReverseY;
}

// BGM�̉��ʂ̐ݒ�
// �ő�l�𒴂����0�ɖ߂�
void SaveData::SetBgmVolume()
{
	data_.volumeBgm++;
	if (data_.volumeBgm > common::config_volume_num)
	{
		data_.volumeBgm = 0;
	}

	// �Z�[�u�f�[�^�ɏ�������
	SaveData::GetInstance().Write();
}

// SE�̉��ʂ̐ݒ�
// �ő�l�𒴂����0�ɖ߂�
void SaveData::SetSeVolume()
{
	data_.volumeSe++;
	if (data_.volumeSe > common::config_volume_num)
	{
		data_.volumeSe = 0;
	}

	// �Z�[�u�f�[�^�ɏ�������
	SaveData::GetInstance().Write();
}

// �p�b�h�̃X�e�B�b�N��X�����x�̐ݒ�
// �ő�l�𒴂����0�ɖ߂�
void SaveData::SetPadStickSensitivityX()
{
	data_.padStickSensitivityX++;
	if (data_.padStickSensitivityX > 10)
	{
		data_.padStickSensitivityX = 0;
	}
	// �Z�[�u�f�[�^�ɏ�������
	SaveData::GetInstance().Write();
}

// �p�b�h�̃X�e�B�b�N��Y�����x�̐ݒ�
// �ő�l�𒴂����0�ɖ߂�
void SaveData::SetPadStickSensitivityY()
{
	data_.padStickSensitivityY++;
	if (data_.padStickSensitivityY > 10)
	{
		data_.padStickSensitivityY = 0;
	}
	// �Z�[�u�f�[�^�ɏ�������
	SaveData::GetInstance().Write();
}

// �p�b�h�̃X�e�B�b�N��X�����o�[�X�̐ݒ�
void SaveData::SetPadStickReverseX()
{
	if (data_.padStickReverseX)
	{
		data_.padStickReverseX = false;
	}
	else
	{
		data_.padStickReverseX = true;
	}
	// �Z�[�u�f�[�^�ɏ�������
	SaveData::GetInstance().Write();
}

// �p�b�h�̃X�e�B�b�N��Y�����o�[�X�̐ݒ�
void SaveData::SetPadStickReverseY()
{
	if (data_.padStickReverseY)
	{
		data_.padStickReverseY = false;
	}
	else
	{
		data_.padStickReverseY = true;
	}
	// �Z�[�u�f�[�^�ɏ�������
	SaveData::GetInstance().Write();
}