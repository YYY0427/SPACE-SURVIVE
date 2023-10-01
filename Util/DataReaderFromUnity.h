#pragma once
#include <string>
#include <DxLib.h>
#include <vector>

// Unity�Ŕz�u�����I�u�W�F�N�g�̃f�[�^��ǂݎ��N���X
class DataReaderFromUnity
{
private:
	// 
	struct UnityGameObject
	{
		std::string name;
		VECTOR pos;
		VECTOR rot;
	};
public:
	// �R���X�g���N�^
	DataReaderFromUnity();

	// �f�X�g���N�^
	virtual ~DataReaderFromUnity();

	// Unity�Ŕz�u�����I�u�W�F�N�g�̃f�[�^��ǂݎ��
	void LoadUnityGameObjectData();

	/// <summary>
	/// �x���@����ʓx�@(180->��)
	/// </summary>
	/// <param name="degree">�ϊ��������p�x</param>
	/// <returns>�ϊ���̊p�x</returns>
	float RadianFromDegree(float degree);

	// �ǂݎ�����f�[�^�̎擾
	std::vector<UnityGameObject> GetData() const;
private:
	// 
	std::vector<UnityGameObject> data_;
};