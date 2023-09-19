#include "Effekseer3DEffectManager.h"
#include "EffekseerForDXLib.h"
#include "DrawFunctions.h"
#include <cassert>

namespace
{
	// �G�t�F�N�g�̃t�@�C���p�X
	const std::string data_file_path = "Data/Effect/";

	// �G�t�F�N�g�̊g���q
	const std::string data_extension = ".efkefc";
}

/// <summary>
/// �R���X�g���N�^
/// </summary>
Effekseer3DEffectManager::Effekseer3DEffectManager():
	imgHandle_(-1)
{
	
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
Effekseer3DEffectManager::~Effekseer3DEffectManager()
{
}

/// <summary>
/// Effekseer3DEffectManager�̗B��̃C���X�^���X��Ԃ�
/// </summary>
/// <returns>�B��̎��Ԃ̎Q��</returns>
Effekseer3DEffectManager& Effekseer3DEffectManager::GetInstance()
{
	// �B��̎���
	static Effekseer3DEffectManager instance;

	// �B��̎��ԎQ�Ƃ�Ԃ�
	return instance;
}

/// <summary>
/// Effekseer�̏������ƃG�t�F�N�g�̃��[�h
/// </summary>
/// <returns> 0 : �������̐����A-1 : �������̎��s  </returns>
int Effekseer3DEffectManager::Init()
{
	// Effekseer������������
	// �����ɂ͉�ʂɕ\������ő�p�[�e�B�N������ݒ肷��
	if (Effkseer_Init(8000) == -1)
	{
		// �������̎��s
		return -1;
	}

	// �摜�̃��[�h
	imgHandle_ = my::MyLoadGraph("Data/Effect/background.png");

	// �������火��Effect�����[�h����
	LoadEffectFile("explosion");

	// �������̐���
	return 0;
}

/// <summary>
/// ���t���[���X�V����
/// </summary>
void Effekseer3DEffectManager::Update()
{
	// DX���C�u�����̃J������Effekseer�̃J�����𓯊�����
	Effekseer_Sync3DSetting();

	// Effekseer�ɂ��Đ����̃G�t�F�N�g���X�V����
	UpdateEffekseer3D();
}

/// <summary>
/// ���t���[���̕`�揈��
/// </summary>
void Effekseer3DEffectManager::Draw()
{
	// ���ł������̂ŉ摜��`�悷��
	// �������ĕ`�悵����łȂ��ƁAEffekseer�͕`��ł��Ȃ�
	DrawGraph(0, 0, imgHandle_, true);

	// Effekseer�ɂ��Đ����̃G�t�F�N�g��`�悷�� 
	DrawEffekseer3D();
}

/// <summary>
/// �I������
/// </summary>
void Effekseer3DEffectManager::End()
{
	// �摜�̍폜
	DeleteGraph(imgHandle_);

	// �G�t�F�N�g���\�[�X���폜����(Effekseer�I�����ɔj�������̂ō폜���Ȃ��Ă�����)
	for (auto& effectResource : effectResourceNameAndHandleTable_)
	{
		DeleteEffekseerEffect(effectResource.second);
	}

	// Effekseer���I������
	Effkseer_End();
}

/// <summary>
/// �G�t�F�N�g�̃��[�h
/// </summary>
/// <param name="fileName">���[�h�������G�t�F�N�g�̃t�@�C����(�g���q�͊܂܂Ȃ�)</param>
void Effekseer3DEffectManager::LoadEffectFile(std::string fileName)
{
	std::string path = data_file_path;
	path += fileName;
	path += data_extension;

	// �G�t�F�N�g�̃��[�h(���s������~�߂�)
	int handle = LoadEffekseerEffect(path.c_str());
	assert(handle != -1);	// -1�ȊO����Ȃ�������~�߂�

	// ���[�h�����G�t�F�N�g�̃n���h���̕ۑ�
	effectResourceNameAndHandleTable_[fileName] = handle;
}

/// <summary>
/// �w��̃G�t�F�N�g�̍Đ�
/// </summary>
/// <param name="fileName">�Đ��������G�t�F�N�g�̃t�@�C����(�g���q�͊܂܂Ȃ�)</param>
/// <param name="pos">�ʒu</param>
/// <param name="scale">�g�嗦</param>
/// <param name="speed">�Đ����x</param>
/// <param name="rot">��]</param>
void Effekseer3DEffectManager::PlayEffect(std::string fileName, VECTOR pos, VECTOR scale, float speed, VECTOR rot)
{
	// �G�t�F�N�g���\�[�X�Ɏw�肵���G�t�F�N�g�����[�h����Ă��Ȃ��ꍇ�~�߂�
	assert(effectResourceNameAndHandleTable_.find(fileName) != effectResourceNameAndHandleTable_.end());

	// �G�t�F�N�g�̍Đ�(���s������~�߂�)
	playingEffectNameAndHandleTable_[fileName] = PlayEffekseer3DEffect(effectResourceNameAndHandleTable_[fileName]);
	assert(playingEffectNameAndHandleTable_[fileName] != -1); // -1�ȊO����Ȃ�������~�߂�

	// �G�t�F�N�g�̍Đ����x��ݒ�
	SetSpeedPlayingEffekseer3DEffect(playingEffectNameAndHandleTable_[fileName], speed);

	// �G�t�F�N�g�̊g�嗦�̐ݒ�
	SetScalePlayingEffekseer3DEffect(playingEffectNameAndHandleTable_[fileName], scale.x, scale.y, scale.z);

	// �G�t�F�N�g�̉�]���̐ݒ�
	SetRotationPlayingEffekseer3DEffect(playingEffectNameAndHandleTable_[fileName], rot.x, rot.y, rot.z);

	// �G�t�F�N�g�̈ʒu�̐ݒ�
	SetPosPlayingEffekseer3DEffect(playingEffectNameAndHandleTable_[fileName], pos.x, pos.y, pos.z);
}

/// <summary>
/// ����̃G�t�F�N�g���Đ�����
/// </summary>
/// <param name="fileName">�Đ��������G�t�F�N�g�̃t�@�C����(�g���q�͊܂܂Ȃ�)</param>
/// <returns>true : �Đ����Afalse : �Đ����Ă��Ȃ�</returns>
bool Effekseer3DEffectManager::IsPlayingEffect(std::string fileName)
{
	if (IsEffekseer3DEffectPlaying(playingEffectNameAndHandleTable_[fileName]) == 0)
	{
		return true;
	}
	return false;
}

/// <summary>
/// �G�t�F�N�g�S�Ă̍Đ����X�g�b�v
/// </summary>
void Effekseer3DEffectManager::StopAllEffect()
{
	for (auto& effect : playingEffectNameAndHandleTable_)
	{
		StopEffekseer3DEffect(effect.second);
	}
}
