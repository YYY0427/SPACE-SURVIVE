#pragma once
#include <unordered_map>
#include <string>
#include <DxLib.h>

// �T�E���h�̊Ǘ����s���V���O���g���N���X
class SoundManager
{
public:
	// �f�X�g���N�^
	~SoundManager();

	/// <summary>
	/// �B��̃C���X�^���X��Ԃ�
	/// </summary>
	/// <returns>�B��̎��Ԃ̎Q��</returns>
	static SoundManager& GetInstance();

	// �t�@�C������T�E���h�̃f�[�^��ǂݎ���ăf�[�^�e�[�u���Ɋi�[
	void LoadAndSaveSoundFileData();

	/// <summary>
	/// �w���2DSE��炷
	/// �T�E���h�����[�h����Ă��Ȃ��ꍇ�A2DSE�ȊO�̏ꍇ�͎~�܂�
	/// </summary>
	/// <param name="name">�Đ��������T�E���h�̃t�@�C����(�g���q�͊܂܂Ȃ�)</param>
	void Play(std::string fileName);

	/// <summary>
	/// �w���3DSE��炷
	/// �T�E���h�����[�h����Ă��Ȃ��ꍇ�A3DSE�ȊO�̏ꍇ�͎~�܂�
	/// </summary>
	/// <param name="fileName">�Đ��������T�E���h�̃t�@�C����</param>
	/// <param name="soundPos">�Đ��ʒu</param>
	/// <param name="soundRadius">�������鋗��</param>
	void Play3D(std::string fileName, VECTOR soundPos, float soundRadius);

	/// <summary>
	/// �w���BGM��炷
	/// �T�E���h�����[�h����Ă��Ȃ��ꍇ�ABGM�ȊO�̏ꍇ�͎~�܂�
	/// </summary>
	/// <param name="fileName">�Đ��������T�E���h�̃t�@�C����</param>
	void PlayBGM(std::string fileName);

	/// <summary>
	/// ����̃T�E���h���Đ������`�F�b�N
	/// �T�E���h�����[�h����Ă��Ȃ�������~�߂�
	/// </summary>
	/// <param name="fileName">�Đ����Ă��邩�`�F�b�N�������T�E���h�̃t�@�C����</param>
	/// <returns>true : �Đ����Afalse : �Đ����Ă��Ȃ�</returns>
	bool PlayingCheckSound(std::string fileName);

	/// <summary>
	/// ����̃T�E���h���~�߂�
	/// �T�E���h�����[�h����Ă��Ȃ�������~�߂�
	/// </summary>
	/// <param name="fileName">�~�߂����T�E���h�̃t�@�C����(�g���q�͊܂܂Ȃ�)</param>
	void StopSound(std::string fileName);

	// ���ׂẴT�E���h���~�߂�
	void StopAllSound();

	/// <summary>
	/// ���ʒ���
	/// �ݒ肵�������ʂƃT�E���h�ɐݒ肳�ꂽ���ʂƃR���t�B�O�Őݒ肳�ꂽ���ʂ��狁�߂��ŏI�I�ȉ��ʂɒ���
	/// </summary>
	/// <param name="fileName">���ʒ��߂������T�E���h�̃t�@�C����(�g���q�͊܂܂Ȃ�)</param>
	/// <param name="volume">�ݒ肵��������(0~255)</param>
	void SetVolume(std::string fileName, int volume);

	/// <summary>
	/// 3D�T�E���h�̃��X�i�[�̈ʒu�ƃ��X�i�[�̑O���ʒu��ݒ肷��
	/// </summary>
	/// <param name="pos">���X�i�[�̍��W</param>
	/// <param name="angle">���X�i�[�̌����Ă���p�x(���W�A��)</param>
	void Set3DSoundListenerPosAndFrontPos_UpVecY(VECTOR pos, VECTOR angle);
private:
	// �T�E���h�̎��
	enum class SoundType
	{
		BGM,
		SE2D,
		SE3D,
	};
	// �T�E���h�̃f�[�^�̎��
	enum class SoundDataType
	{
		FILE_NAME,
		EXTENSION,
		SOUND_TYPE,
		VOLUM_RATE,
	};
private:
	// �T�E���h�̃f�[�^
	struct SoundData
	{
		SoundType type;		// BGM��3D��SE��2D��SE��
		std::string extension;	// �T�E���h�t�@�C���̊g���q
		float volumeRate;	// �{�����[������
		int handle;			// �n���h��
	};
private:
	// �R���X�g���N�^(�V���O���g���p�^�[���Ȃ̂�private�ɒu��)
	SoundManager();

	// �R�s�[��������֎~����
	SoundManager(const SoundManager&) = delete;			// �R�s�[�R���X�g���N�^
	void operator = (const SoundManager&) = delete;		// ������֎~

	/// <summary>
	/// 2D�T�E���h�̃��[�h
	/// ���[�h�Ɏ��s������~�߂�
	/// </summary>
	/// <param name="fileName">���[�h�������T�E���h�t�@�C����(�g���q�Ȃ�)</param>
	/// <param name="extension">���[�h�����T�E���h�̊g���q</param>
	void LoadSoundFile2D(std::string fileName, std::string ext);

	/// <summary>
	/// 3D�T�E���h�̃��[�h
	/// ���[�h�Ɏ��s������~�߂�
	/// </summary>
	/// <param name="fileName">���[�h�������T�E���h�t�@�C����(�g���q�Ȃ�)</param>
	/// <param name="extension">���[�h�����T�E���h�̊g���q</param>
	void LoadSoundFile3D(std::string fileName, std::string ext);
private:
	// ���[�h�����T�E���h�̃t�@�C�����ƃn���h��
	std::unordered_map<std::string, SoundData> soundNameAndHandleTable_;
};