#include "DataReaderFromUnity.h"
#include <cassert>

namespace
{
	// �t�@�C���̃p�X
	const std::string data_file_path = "Data/ObjectData.dat";

	const std::string player_data_name = "Player";
	const std::string rock_data_name = "Rock";
	const std::string meteor_data_name = "Meteor";
}

// �R���X�g���N�^
DataReaderFromUnity::DataReaderFromUnity() 
{
}

// �f�X�g���N�^
DataReaderFromUnity::~DataReaderFromUnity()
{
}

// Unity�Ŕz�u�����I�u�W�F�N�g�̃f�[�^��ǂݎ��
void DataReaderFromUnity::LoadUnityGameObjectData()
{
	// �t�@�C���ɃA�N�Z�X
	auto dataHandle = FileRead_open(data_file_path.c_str());

	// �f�[�^���𓾂�
	int dataNum = 0;
	int result = FileRead_read(&dataNum, sizeof(dataNum), dataHandle);
	assert(result != -1);

	// �f�[�^�̓ǂݎ��
	for (int i = 0; i < dataNum; i++)
	{
		UnityGameObject data;

		// ���O�̕����񐔂̓ǂݎ��
		uint8_t nameSize = 0;
		result = FileRead_read(&nameSize, sizeof(nameSize), dataHandle);
		assert(result != -1);

		// ���O���̂��̂�ǂݎ��
		data.name.resize(nameSize);
		result = FileRead_read(data.name.data(), sizeof(char) * nameSize, dataHandle);
		assert(result != -1);

		// ���W�f�[�^xyz��ǂ�
		result = FileRead_read(&data.pos, sizeof(data.pos), dataHandle);
		assert(result != -1);

		// Unity�Ƃ̍��W�f�[�^�����킹�邽�߂�100�{
		data.pos = VScale(data.pos, 100);

		// ��]�f�[�^xyz��ǂ�
		result = FileRead_read(&data.rot, sizeof(data.rot), dataHandle);
		assert(result != -1);

		// �x���@���ʓx�@�ɕϊ�
		data.rot.x = RadianFromDegree(data.rot.x);
		data.rot.y = RadianFromDegree(data.rot.y);
		data.rot.z = RadianFromDegree(data.rot.z);

		// �g��f�[�^xyz��ǂ�
		result = FileRead_read(&data.scale, sizeof(data.scale), dataHandle);
		assert(result != -1);

		// ���O�ɂ���ĕۑ�����ϐ���ς���
		if (data.name == player_data_name)
		{
			playerData_ = data;
		}
		else if (data.name == rock_data_name)
		{
			rockData_.push_back(data);
		}
		else if (data.name == meteor_data_name)
		{
			meteorData_.push_back(data);
		}
		else
		{
			// ���肦�Ȃ��̂Ŏ~�߂�
			assert(0);
		}
	}
	// �t�@�C�������
	FileRead_close(dataHandle);
}

// �x���@����ʓx�@(180->��)
float DataReaderFromUnity::RadianFromDegree(float degree)
{
	return DX_PI_F * degree / 180.0f;
}

// ��̃f�[�^�̎擾
std::vector<UnityGameObject> DataReaderFromUnity::GetRockData() const
{
	return rockData_;
}

// 覐΂̃f�[�^�̎擾
std::vector<UnityGameObject> DataReaderFromUnity::GetMeteorData() const
{
	return meteorData_;
}

// �v���C���[�̃f�[�^�̎擾
UnityGameObject DataReaderFromUnity::GetPlayerData() const
{
	return playerData_;
}