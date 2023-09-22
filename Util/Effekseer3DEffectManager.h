#pragma once
#include <DxLib.h>
#include <string>
#include <unordered_map>

// Effekseer���g����3D�G�t�F�N�g�̊Ǘ����s���V���O���g���N���X
class Effekseer3DEffectManager
{
public:
	// �f�X�g���N�^
	~Effekseer3DEffectManager();

	/// <summary>
	/// Effekseer3DEffectManager�̗B��̃C���X�^���X��Ԃ�
	/// </summary>
	/// <returns>�B��̎��Ԃ̎Q��</returns>
	static Effekseer3DEffectManager& GetInstance();

	// Effekseer�̏������ƃG�t�F�N�g�̃��[�h
	// �������Ɏ��s������~�߂�
	void Init();

	// �X�V
	void Update();

	// �`��
	void Draw();

	// �I������
	void End();

	/// <summary>
	/// �w��̃G�t�F�N�g�̍Đ�
	/// </summary>
	/// <param name="fileName">�Đ��������G�t�F�N�g�̃t�@�C����(�g���q�͊܂܂Ȃ�)</param>
	/// <param name="pos">�ʒu</param>
	/// <param name="scale">�g�嗦</param>
	/// <param name="speed">�Đ����x</param>
	/// <param name="rot">��]</param>
	void PlayEffect(std::string fileName, VECTOR pos, float scale, float speed, VECTOR rot = VGet(0, 0, 0));

	void SetPosPlayingEffect(std::string fileName, VECTOR pos);
	void SetScalePlayingEffect(std::string fileName, float  scale);
	void SetSpeedPlayingEffect(std::string fileName, float speed);
	void SetRotPlayingEffect(std::string fileName, VECTOR rot);

	/// <summary>
	/// ����̃G�t�F�N�g���Đ�����
	/// </summary>
	/// <param name="fileName">�Đ��������G�t�F�N�g�̃t�@�C����(�g���q�͊܂܂Ȃ�)</param>
	/// <returns>true : �Đ����Afalse : �Đ����Ă��Ȃ�</returns>
	bool IsPlayingEffect(std::string fileName);

	/// <summary>
	/// ����̃G�t�F�N�g�̍Đ����X�g�b�v
	/// </summary>
	/// <param name="fileName">�X�g�b�v�������G�t�F�N�g�̃t�@�C����(�g���q�͊܂܂Ȃ�)</param>
	void StopEffect(std::string fileName);

	// �G�t�F�N�g�S�Ă̍Đ����X�g�b�v
	void StopAllEffect();

private:
	// �R���X�g���N�^(�V���O���g���p�^�[���Ȃ̂�private�ɒu��)
	Effekseer3DEffectManager();

	// �R�s�[��������֎~����
	Effekseer3DEffectManager(const Effekseer3DEffectManager&) = delete;	// �R�s�[�R���X�g���N�^�̋֎~
	void operator = (const Effekseer3DEffectManager&) = delete;			// ����̋֎~

	/// <summary>
	/// �G�t�F�N�g�̃��[�h
	/// </summary>
	/// <param name="fileName">���[�h�������G�t�F�N�g�̃t�@�C����(�g���q�͊܂܂Ȃ�)</param>
	void LoadEffectFile(std::string fileName);

private:
	// ���[�h�����G�t�F�N�g�̃t�@�C�����ƃn���h��
	std::unordered_map<std::string, int> effectResourceNameAndHandleTable_;

	// �Đ����̃G�t�F�N�g�̃t�@�C�����ƃn���h��
	std::unordered_map<std::string, int> playingEffectNameAndHandleTable_;

	// �摜�n���h��
	int imgHandle_;
};