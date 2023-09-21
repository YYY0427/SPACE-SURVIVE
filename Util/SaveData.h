#pragma once

// �Z�[�u�f�[�^�̊Ǘ����s���V���O���g���N���X
class SaveData
{
public:
	// �f�X�g���N�^
	~SaveData();

	/// <summary>
	/// �B��̃C���X�^���X��Ԃ�
	/// </summary>
	/// <returns>�B��̎��Ԃ̎Q��</returns>
	static SaveData& GetInstance();

	// �Z�[�u�f�[�^�̓ǂݍ���
	void Load();

	// �Z�[�u�f�[�^�̏�������
	// �Z�[�u�f�[�^�̃o�[�W��������v���Ȃ��ꍇ�͎~�߂�
	void Write();

	// �Z�[�u�f�[�^��V�K�쐬���ď㏑��
	void CreateNewData();

	// ���ʂ̎擾
	int GetBgmVolume() const;	// BGM
	int GetSeVolume() const;	// SE
	int GetPadStickSensitivityX() const;
	int GetPadStickSensitivityY() const;

	// ���ʐݒ�
	// ���ʂ̐ݒ�(�ő�l�𒴂����0�ɖ߂�)
	void SetBgmVolume();	// BGM
	void SetSeVolume();		// SE
	void SetPadStickSensitivityX();	// �p�b�h�X�e�B�b�N�̊��xX
	void SetPadStickSensitivityY();	// �p�b�h�X�e�B�b�N�̊��xY
private:
	// �Z�[�u�f�[�^
	struct Data
	{
		int version = 0;		// �f�[�^�̃o�[�W����
		int volumeBgm = 3;		// BGM����	
		int volumeSe = 3;		// SE����	
		int padStickSensitivityX = 5;	// �p�b�h�̃X�e�B�b�N�̊��xX
		int padStickSensitivityY = 3;	// �p�b�h�̃X�e�B�b�N�̊��xY
	};
private:
	// �R���X�g���N�^(�V���O���g���p�^�[���Ȃ̂�private�ɒu��)
	SaveData();

	// �R�s�[��������֎~
	SaveData(const SaveData&) = delete;			// �R�s�[�R���X�g���N�^
	void operator = (const SaveData&) = delete;	// ������֎~

	// �f�[�^�̍폜
	void InitData();

private:
	// �Z�[�u�f�[�^
	Data data_;
};