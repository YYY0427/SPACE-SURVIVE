#pragma once
#include <DxLib.h>
#include <string>
#include <unordered_map>
#include <list>

namespace EffectID
{
	const std::string player_dead = "explosion2";				// �v���C���[���S���ɏo���G�t�F�N�g
	const std::string player_boost = "starFire";				// �v���C���[�u�[�X�g���Ɍp���I�ɏo���G�t�F�N�g
	const std::string player_acceleration = "acceleration";		// �v���C���[�u�[�X�g���Ɉ�x�����o���G�t�F�N�g
	const std::string meteor = "boost";							// 覐Ηp�G�t�F�N�g
}

// �G�t�F�N�g�̍Đ��^�C�v
enum class PlayType
{
	NORMAL,
	FOLLOW,
	LOOP,
	LOOP_AND_FOLLOW,
};

struct EffectData
{
	std::string effectFileName;
	int* playingEffectHandle;
	PlayType type;
	VECTOR* pos;
	VECTOR* rot;
	float* scale;
	float* speed;
};

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
	/// <param name="isForcePlay">���łɎw�肳�ꂽ�G�t�F�N�g���Đ�����Ă���ꍇ���Đ����邩</param>
	/// <param name="pos">�ʒu</param>
	/// <param name="scale">�g�嗦</param>
	/// <param name="speed">�Đ����x</param>
	/// <param name="rot">��]</param>
	void PlayEffect(int* playingEffectHandle, std::string fileName, PlayType type, VECTOR* pos, float* scale, float* speed, VECTOR* rot);
	void PlayEffectLoop(int* playingEffectHandle, std::string fileName);

	void SetEffectParam(int playingEffectHandle, VECTOR pos, float scale, float speed, VECTOR rot);

	/// <summary>
	/// ����̃G�t�F�N�g���Đ�����
	/// </summary>
	/// <param name="fileName">�Đ��������G�t�F�N�g�̃t�@�C����(�g���q�͊܂܂Ȃ�)</param>
	/// <returns>true : �Đ����Afalse : �Đ����Ă��Ȃ�</returns>
	bool IsPlayingEffect(int effectPlayingHandle);

	/// <summary>
	/// ����̃G�t�F�N�g�̍Đ����X�g�b�v
	/// </summary>
	/// <param name="fileName">�X�g�b�v�������G�t�F�N�g�̃t�@�C����(�g���q�͊܂܂Ȃ�)</param>
	void StopEffect(int effectPlayingHandle);

	// �G�t�F�N�g�S�Ă̍Đ����X�g�b�v
	void StopAllEffect();

	void DeleteEffect(int effectPlayingHandle);
	void DeleteAllEffect();

private:
	// �R���X�g���N�^
	// �V���O���g���p�^�[���Ȃ̂�private
	Effekseer3DEffectManager();

	// �R�s�[��������֎~
	Effekseer3DEffectManager(const Effekseer3DEffectManager&) = delete;	// �R�s�[�R���X�g���N�^�֎~
	void operator = (const Effekseer3DEffectManager&) = delete;			// ����֎~

	/// <summary>
	/// �G�t�F�N�g�̃��[�h
	/// </summary>
	/// <param name="fileName">���[�h�������G�t�F�N�g�̃t�@�C����(�g���q�͊܂܂Ȃ�)</param>
	void LoadEffectFile(std::string fileName);

private:
	// �G�t�F�N�g�̃t�@�C������ID�Ƃ����G�t�F�N�g���\�[�X�̃n���h���̃e�[�u��
	std::unordered_map<std::string, int> effectResourceHandleTable_;

	// �G�t�F�N�g�̃f�[�^��ۑ������e�[�u��
	std::list<EffectData> effectDataTable_;

	// �摜�n���h��
	int imgHandle_;
};