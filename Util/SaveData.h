#pragma once

/// <summary>
/// �Z�[�u�f�[�^�̊Ǘ����s���V���O���g���N���X
/// </summary>
class SaveData
{
public:
	// �f�X�g���N�^
	~SaveData();

	// SaveData�̗B��̃C���X�^���X��Ԃ�
	static SaveData& GetInstance();

	// �Z�[�u�f�[�^�̓ǂݍ���
	void Load();

	// �Z�[�u�f�[�^�̏�������
	void Write();

	// �Z�[�u�f�[�^��V�K�쐬���ď㏑��
	void CreateNewData();

	// �Q�b�^�[
	int GetBgmVolume() const;	// BGM����	0~5
	int GetSeVolume() const;	// SE����	0~5

private:
	// �Z�[�u�f�[�^
	struct Data
	{
		int version = 0;		// �f�[�^�̃o�[�W����
		int volumeBgm = 3;		// BGM����	0~5
		int volumeSe = 3;		// SE����	0~5
	};
private:
	// �R���X�g���N�^(�V���O���g���p�^�[���Ȃ̂�private�ɒu��)
	SaveData();

	// �R�s�[��������֎~����
	SaveData(const SaveData&) = delete;			// �R�s�[�R���X�g���N�^
	void operator = (const SaveData&) = delete;	// ������֎~

	// �f�[�^�̍폜
	void InitData();

private:
	Data data_;
};