#include "DataReaderFromUnity.h"
#include <cassert>

namespace
{
	// �t�@�C���̃p�X
	const std::string data_file_path = "Data/ObjectData.dat";
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
		data.pos = VScale(data.pos, 100.0f);

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
		
		// �I�u�W�F�N�g�̖��O���Ƃɕۑ�
		data_[data.name].push_back(data);
	}

	// �t�@�C�������
	FileRead_close(dataHandle);
}

// �x���@����ʓx�@(180->��)
float DataReaderFromUnity::RadianFromDegree(float degree)
{
	return DX_PI_F * degree / 180.0f;
}

std::vector<UnityGameObject> DataReaderFromUnity::GetDataType(std::string objectName) const
{
	// �f�[�^������������
	if (data_.find(objectName) != data_.end())
	{
		return data_.find(objectName)->second;
	}

	// ������Ȃ��������̃f�[�^��Ԃ�
	return std::vector<UnityGameObject>();
}