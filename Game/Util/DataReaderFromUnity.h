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
	std::unordered_map<std::string, std::vector<UnityGameObject>> GetData() const;

private:
	// Unity�ł̃I�u�W�F�N�g�̖��O��ID�Ƃ���Unity�œǂݎ�����f�[�^�̃f�[�^�^�C�v�̃e�[�u��
	std::unordered_map<std::string, std::vector<UnityGameObject>> data_;
};