#include "SaveData.h"
#include "../Game.h"
#include <string>
#include <cassert>
#include <fstream>
#include <DxLib.h>

using namespace std;

namespace
{
	// �Z�[�u�f�[�^�t�@�C����
	const string data_file_path = "Data/Save/Data.dat";

	// �Z�[�u�f�[�^�o�[�W�����@
	constexpr int current_save_version = 0;
}

/// <summary>
/// �R���X�g���N�^
/// </summary>
SaveData::SaveData()
{

}

/// <summary>
/// �f�X�g���N�^
/// </summary>
SaveData::~SaveData()
{
}

/// <summary>
/// SaveData�̗B��̃C���X�^���X��Ԃ�
/// </summary>
/// <returns>�B��̎��Ԃ̎Q��</returns>
SaveData& SaveData::GetInstance()
{
	// �B��̎���
	static SaveData instance;

	// �B��̎��ԎQ�Ƃ�Ԃ�
	return instance;
}

/// <summary>
/// �Z�[�u�f�[�^�̓ǂݍ���
/// </summary>
void SaveData::Load()
{
	// �Z�[�u�f�[�^�̏�����
	InitData();

	std::ifstream ifs;
	ifs.open(data_file_path, std::ios_base::binary);
	if (ifs.fail())
	{
		// �t�@�C���ǂݍ��ݎ��s �Z�[�u�f�[�^�����
		CreateNewData();
		return;
	}
	else
	{
		Data data;
		ifs.read((char*)&data, sizeof(Data));
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

/// <summary>
/// �Z�[�u�f�[�^�̏�������
/// </summary>
void SaveData::Write()
{
	assert(data_.version == current_save_version);
	std::ofstream ofs(data_file_path, std::ios_base::binary);
	ofs.write((char*)&data_, sizeof(Data));
}

/// <summary>
/// �Z�[�u�f�[�^��V�K�쐬���ď㏑��
/// </summary>
void SaveData::CreateNewData()
{
	// �Z�[�u�f�[�^�̏�����
	InitData();

	std::ofstream ofs(data_file_path, std::ios_base::binary);
	ofs.write((char*)&data_, sizeof(SaveData));
}

/// <summary>
/// �Z�[�u�f�[�^�̏�����
/// </summary>
void SaveData::InitData()
{
	data_.version = current_save_version;
	data_.volumeBgm = 3;
	data_.volumeSe = 3;
}

/// <summary>
/// BGM�̉��ʂ̎擾(0~5)
/// </summary>
/// <returns>BGM�̉���(0~5)</returns>
int SaveData::GetBgmVolume() const
{
	return data_.volumeBgm;
}

/// <summary>
/// SE�̉��ʂ̎擾(0~5)
/// </summary>
/// <returns>SE�̉���(0~5)</returns>
int SaveData::GetSeVolume() const
{
	return data_.volumeSe;
}

/// <summary>
/// BGM�̉��ʂ̐ݒ�(�ő�l�𒴂����0�ɖ߂�)
/// </summary>
void SaveData::SetBgmVolume()
{
	data_.volumeBgm++;
	if (data_.volumeBgm > Game::config_volume_num)
	{
		data_.volumeBgm = 0;
	}

	// �Z�[�u�f�[�^�ɏ�������
	SaveData::GetInstance().Write();
}

/// <summary>
/// SE�̉��ʂ̐ݒ�(�ő�l�𒴂����0�ɖ߂�)
/// </summary>
void SaveData::SetSeVolume()
{
	data_.volumeSe++;
	if (data_.volumeSe > Game::config_volume_num)
	{
		data_.volumeSe = 0;
	}

	// �Z�[�u�f�[�^�ɏ�������
	SaveData::GetInstance().Write();
}