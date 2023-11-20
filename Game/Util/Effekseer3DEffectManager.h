#pragma once
#include <DxLib.h>
#include <string>
#include <unordered_map>
#include <list>

// �G�t�F�N�g�̃t�@�C���p�X
namespace EffectID
{
	const std::string player_dead = "Explosion";		// �v���C���[���S���ɏo���G�t�F�N�g
	const std::string player_shield = "MagicShield";	// �v���C���[�̃V�[���h	
	const std::string player_boost = "FirePot1";		// �v���C���[�ɏ�ɏo���G�t�F�N�g
	const std::string normal_lazer = "blue_lazer";		// �G�̒ʏ탌�[�U�[�̃G�t�F�N�g
	const std::string refrect_laser = "RefrectLaser";	// �G�̃��[�U�[�𔽎˂������[�U�[
	const std::string enemy_died = "Explosion";			// �G�̎��S���̃G�t�F�N�g
	const std::string enemy_boss_entry = "EnemyBossEntry";	// �G�̃{�X���o������Ƃ��̃G�t�F�N�g
	const std::string wind = "Wind";

	const std::string effect_file_path[] =
	{
		player_dead,
		player_shield,
		player_boost,
		normal_lazer,
		refrect_laser,
		enemy_died,
		enemy_boss_entry,
		wind,
	};
}

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
	/// <param name="playingEffectHandle">�G�t�F�N�g�̃n���h��(�Ԃ��Ă����G�t�F�N�g�n���h�����g���Ēl�̕ύX���s��)</param>
	/// <param name="fileName">�Đ��������G�t�F�N�g�̃t�@�C����(�g���q�͊܂܂Ȃ�)</param>
	/// <param name="pos">�ʒu</param>
	/// <param name="scale">�g�嗦</param>
	/// <param name="speed">�Đ����x</param>
	/// <param name="rot">��]��</param>
	void PlayEffect(int& playingEffectHandle, std::string fileName, VECTOR pos, float scale = 1.0f, float speed = 1.0f, VECTOR rot = { 0.0f, 0.0f, 0.0f });

	/// <summary>
	/// �w��̃G�t�F�N�g�̃��[�v�Đ�
	/// </summary>
	/// <param name="playingEffectHandle">�G�t�F�N�g�̃n���h��(�Ԃ��Ă����G�t�F�N�g�n���h�����g���Ēl�̕ύX���s��)</param>
	/// <param name="fileName">�Đ��������G�t�F�N�g�̃t�@�C����(�g���q�͊܂܂Ȃ�)</param>
	/// <param name="pos">�ʒu</param>
	/// <param name="scale">�g�嗦</param>
	/// <param name="speed">�Đ����x</param>
	/// <param name="rot">��]��</param>
	void PlayEffectLoop(int& playingEffectHandle, std::string fileName, VECTOR pos, float scale = 1.0f, float speed = 1.0f, VECTOR rot = { 0.0f, 0.0f, 0.0f });

	/// <summary>
	/// �w��̃G�t�F�N�g�̒Ǐ]�Đ�
	/// </summary>
	/// <param name="playingEffectHandle">�G�t�F�N�g�̃n���h��(�Ԃ��Ă����G�t�F�N�g�n���h�����g���Ēl�̕ύX���s��)</param>
	/// <param name="fileName">�Đ��������G�t�F�N�g�̃t�@�C����(�g���q�͊܂܂Ȃ�)</param>
	/// <param name="pos">�Ǐ]�������ʒu���ϐ��̃A�h���X</param>
	/// <param name="scale">�g�嗦</param>
	/// <param name="speed">�Đ����x</param>
	/// <param name="rot">��]��</param>
	void PlayEffectFollow(int& playingEffectHandle, std::string fileName, VECTOR* pos, float scale = 1.0f, float speed = 1.0f, VECTOR rot = { 0.0f, 0.0f, 0.0f });

	/// <summary>
	/// �w��̃G�t�F�N�g�̒Ǐ]�ƃ��[�v�Đ�
	/// </summary>
	/// <param name="playingEffectHandle">�G�t�F�N�g�̃n���h��(�Ԃ��Ă����G�t�F�N�g�n���h�����g���Ēl�̕ύX���s��)</param>
	/// <param name="fileName">�Đ��������G�t�F�N�g�̃t�@�C����(�g���q�͊܂܂Ȃ�)</param>
	/// <param name="pos">�Ǐ]�������ʒu���ϐ��̃A�h���X</param>
	/// <param name="scale">�g�嗦</param>
	/// <param name="speed">�Đ����x</param>
	/// <param name="rot">��]��</param>
	void PlayEffectLoopAndFollow(int& playingEffectHandle, std::string fileName, VECTOR* pos, float scale = 1.0f, float speed = 1.0f, VECTOR rot = { 0.0f, 0.0f, 0.0f });

	/// <summary>
	/// �G�t�F�N�g�̑S�Ă̒l�̕ύX
	/// </summary>
	/// <param name="playingEffectHandle">�G�t�F�N�g�̃n���h��</param>
	/// <param name="pos">�ʒu</param>
	/// <param name="scale">�g�嗦</param>
	/// <param name="speed">�Đ����x</param>
	/// <param name="rot">��]��</param>
	void SetEffectAllParam(int playingEffectHandle, VECTOR pos, float scale, float speed, VECTOR rot);

	/// <summary>
	/// �G�t�F�N�g�̊g�嗦�̕ύX
	/// </summary>
	/// <param name="playingEffectHandle">�G�t�F�N�g�̃n���h��</param>
	/// <param name="scale">�g�嗦</param>
	void SetEffectScale(int playingEffectHandle, float scale);

	/// <summary>
	/// �G�t�F�N�g�̍Đ����x�̕ύX
	/// </summary>
	/// <param name="playingEffectHandle">�G�t�F�N�g�̃n���h��</param>
	/// <param name="speed">�Đ����x</param>
	void SetEffectSpeed(int playingEffectHandle, float speed);

	/// <summary>
	/// �G�t�F�N�g�̉�]���̕ύX
	/// </summary>
	/// <param name="playingEffectHandle">�G�t�F�N�g�̃n���h��</param>
	/// <param name="rot">��]��</param>
	void SetEffectRot(int playingEffectHandle, VECTOR rot);

	/// <summary>
	/// �G�t�F�N�g�̈ʒu�̕ύX
	/// </summary>
	/// <param name="playingEffectHandle">�G�t�F�N�g�̃n���h��</param>
	/// <param name="pos">�ʒu</param>
	void SetEffectPos(int playingEffectHandle, VECTOR pos);

	/// <summary>
	/// ����̃G�t�F�N�g���Đ�����
	/// </summary>
	/// <param name="fileName">�G�t�F�N�g�̃n���h��</param>
	/// <returns>true : �Đ����Afalse : �Đ����Ă��Ȃ�</returns>
	bool IsPlayingEffect(int effectPlayingHandle);

	/// <summary>
	/// ����̃G�t�F�N�g�̍Đ����X�g�b�v
	/// �Đ��^�C�v�����[�v�̃G�t�F�N�g�̏ꍇ�͎~�܂�����A���̃t���[���ōĐ������
	/// </summary>
	/// <param name="fileName">�G�t�F�N�g�̃n���h��</param>
	void StopEffect(int effectPlayingHandle);

	// �G�t�F�N�g�S�Ă̍Đ����X�g�b�v
	// �Đ��^�C�v�����[�v�̃G�t�F�N�g�̏ꍇ�͎~�܂�����A���̃t���[���ōĐ������
	void StopAllEffect();

	/// <summary>
	/// ����̃G�t�F�N�g�̍폜
	/// �ۑ����Ă���f�[�^�e�[�u������폜����
	/// ��ɍĐ��^�C�v�����[�v�̃G�t�F�N�g���폜����̂Ɏg��
	/// </summary>
	/// <param name="effectPlayingHandle">�G�t�F�N�g�̃n���h��</param>
	void DeleteEffect(int effectPlayingHandle);

	// �S�ẴG�t�F�N�g�̍폜
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

	/// <summary>
	/// ���[�v�G�t�F�N�g�̍čĐ�
	/// ���[�v�G�t�F�N�g�̍Đ����I�����Ă�����G�t�F�N�g���Đ�����
	/// </summary>
	/// <param name="playingEffectHandle">�G�t�F�N�g�̃n���h��</param>
	/// <param name="fileName">�čĐ�����G�t�F�N�g�̃t�@�C����(�g���q�͊܂܂Ȃ�)</param>
	void PlayEffectLoop(int& playingEffectHandle, std::string fileName);

private:
	// �G�t�F�N�g�̍Đ��^�C�v
	enum class PlayType
	{
		NORMAL,
		FOLLOW,
		LOOP,
		LOOP_AND_FOLLOW,
	};

private:
	// �G�t�F�N�g�f�[�^
	struct EffectData
	{
		std::string effectFileName;
		int* playingEffectHandle;
		PlayType type;
		VECTOR pos;
		VECTOR rot;
		float scale;
		float speed;
	};

	// �Ǐ]�G�t�F�N�g�f�[�^
	struct FollowEffectData
	{
		std::string effectFileName;
		int* playingEffectHandle;
		PlayType type;
		VECTOR* pos;
		VECTOR rot;
		float scale;
		float speed;
	};

private:
	// �G�t�F�N�g�̃t�@�C������ID�Ƃ����G�t�F�N�g���\�[�X�̃n���h���̃e�[�u��
	std::unordered_map<std::string, int> effectResourceHandleTable_;

	// �G�t�F�N�g�̃f�[�^��ۑ������e�[�u��
	std::list<EffectData> effectDataTable_;

	// �Ǐ]�G�t�F�N�g�̃f�[�^��ۑ������e�[�u��
	std::list<FollowEffectData> followEffectDataTable_;

	// �摜�n���h��
	int imgHandle_;
};