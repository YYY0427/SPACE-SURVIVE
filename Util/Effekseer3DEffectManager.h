#pragma once
#include <DxLib.h>
#include <string>
#include <unordered_map>



/// <summary>
/// Effekseer���g����3D�G�t�F�N�g�̊Ǘ����s���V���O���g���N���X
/// </summary>
class Effekseer3DEffectManager
{
public:
	// �f�X�g���N�^
	~Effekseer3DEffectManager();

	// Effekseer3DEffectManager�̗B��̃C���X�^���X��Ԃ�
	static Effekseer3DEffectManager& GetInstance();

	// Effekseer�̏������ƃG�t�F�N�g�̃��[�h
	int Init();

	// ���t���[���X�V����
	void Update();

	// ���t���[���̕`�揈��
	void Draw();

	// �I������
	void End();

	// �w��̃G�t�F�N�g�̍Đ�
	void PlayEffect(std::string fileName, VECTOR pos, VECTOR scale, float speed, VECTOR rot = VGet(0, 0, 0));

	// ����̃G�t�F�N�g���Đ�����
	bool IsPlayingEffect(std::string fileName);

	// �G�t�F�N�g�S�Ă̍Đ����X�g�b�v
	void StopAllEffect();

private:
	// �R���X�g���N�^(�V���O���g���p�^�[���Ȃ̂�private�ɒu��)
	Effekseer3DEffectManager();

	// �R�s�[��������֎~����
	Effekseer3DEffectManager(const Effekseer3DEffectManager&) = delete;	// �R�s�[�R���X�g���N�^�̋֎~
	void operator = (const Effekseer3DEffectManager&) = delete;			// ����̋֎~

	// �G�t�F�N�g�̃��[�h
	void LoadEffectFile(std::string fileName);

private:
	// ���[�h�����G�t�F�N�g�̃t�@�C�����ƃn���h��
	std::unordered_map<std::string, int> effectResourceNameAndHandleTable_;

	// �Đ����̃G�t�F�N�g�̃t�@�C�����ƃn���h��
	std::unordered_map<std::string, int> playingEffectNameAndHandleTable_;

	// �摜�n���h��
	int imgHandle_;
};