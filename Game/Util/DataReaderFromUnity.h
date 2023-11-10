#pragma once
#include <string>
#include <DxLib.h>
#include <vector>
#include <unordered_map>

struct UnityGameObject
{
	std::string name;
	VECTOR pos;
	VECTOR rot;
	VECTOR scale;
};

// Unity�Ŕz�u�����I�u�W�F�N�g�̃f�[�^��ǂݎ��N���X
class DataReaderFromUnity
{
public:
	// �f�X�g���N�^
	virtual ~DataReaderFromUnity();

	/// <summary>
	/// �B��̃C���X�^���X��Ԃ�
	/// </summary>
	/// <returns>�B��̎��Ԃ̎Q��</returns>
	static DataReaderFromUnity& GetInstance();

	// Unity�Ŕz�u�����I�u�W�F�N�g�̃f�[�^��ǂݎ��
	void LoadUnityGameObjectData(const TCHAR* fileName);

	/// <summary>
	/// �x���@����ʓx�@(180->��)
	/// </summary>
	/// <param name="degree">�ϊ��������p�x</param>
	/// <returns>�ϊ���̊p�x</returns>
	float RadianFromDegree(float degree);

	// �ǂݎ�����f�[�^�̎擾
	std::vector<UnityGameObject> GetData(std::string objectName) const;

private:
	// �R���X�g���N�^
	DataReaderFromUnity();

	// �R�s�[��������֎~
	DataReaderFromUnity(const DataReaderFromUnity&) = delete;	// �R�s�[�R���X�g���N�^�֎~
	void operator = (const DataReaderFromUnity&) = delete;		// ����֎~

	// Unity�ł̃I�u�W�F�N�g�̖��O��ID�Ƃ���Unity�œǂݎ�����f�[�^�̃f�[�^�^�C�v�̃e�[�u��
	std::unordered_map<std::string, std::vector<UnityGameObject>> data_;
};