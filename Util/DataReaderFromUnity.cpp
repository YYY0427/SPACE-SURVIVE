#include "DataReaderFromUnity.h"
#include <cassert>

namespace
{
	// // �t�@�C���̃p�X
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

	// �I�u�W�F�N�g���Ԃ�z����m��
	data_.resize(dataNum);

	// �f�[�^�̓ǂݎ��
	for (int i = 0; i < dataNum; i++)
	{
		auto& data = data_[i];

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

		// ��]�f�[�^xyz��ǂ�
		result = FileRead_read(&data.rot, sizeof(data.rot), dataHandle);
		assert(result != -1);
	}
	FileRead_close(dataHandle);

	// �x���@���ʓx�@�ɕϊ�
	for (auto& object : data_)
	{
		object.rot.x = RadianFromDegree(object.rot.x);
		object.rot.y = RadianFromDegree(object.rot.y);
		object.rot.z = RadianFromDegree(object.rot.z);
	}
}

// �x���@����ʓx�@(180->��)
float DataReaderFromUnity::RadianFromDegree(float degree)
{
	return DX_PI_F * degree / 180.0f;
}

// �ǂݎ�����f�[�^�̎擾
std::vector<DataReaderFromUnity::UnityGameObject> DataReaderFromUnity::GetData() const
{
	return data_;
}