#pragma once
#include <string>
#include <DxLib.h>
#include <vector>

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
	std::vector<UnityGameObject> GetRockData() const;	// ��
	std::vector<UnityGameObject> GetMeteorData() const;	// ��
	UnityGameObject GetPlayerData() const;				// �v���C���[

private:
	// ��̃f�[�^�̕ۑ��p
	std::vector<UnityGameObject> rockData_;

	// 覐΂̃f�[�^�̕ۑ��p
	std::vector<UnityGameObject> meteorData_;

	// �v���C���[�̃f�[�^�̕ۑ��p
	UnityGameObject playerData_;
};