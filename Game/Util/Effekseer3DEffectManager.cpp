#include "Effekseer3DEffectManager.h"
#include "EffekseerForDXLib.h"
#include "DrawFunctions.h"
#include <cassert>

namespace EffectID
{
	std::string player_dead = "explosion2";				// �v���C���[���S���ɏo���G�t�F�N�g
	std::string player_boost = "starFire";				// �v���C���[�u�[�X�g���Ɍp���I�ɏo���G�t�F�N�g
	std::string player_acceleration = "acceleration";	// �v���C���[�u�[�X�g���Ɉ�x�����o���G�t�F�N�g
	std::string meteor = "boost";						// 覐Ηp�G�t�F�N�g
}

namespace
{
	// �G�t�F�N�g�̃t�@�C���p�X
	const std::string data_file_path = "Data/Effect/";

	// �G�t�F�N�g�̊g���q
	const std::string data_extension = ".efk";
}

// �R���X�g���N�^
Effekseer3DEffectManager::Effekseer3DEffectManager():
	imgHandle_(-1)
{
	
}

// �f�X�g���N�^
Effekseer3DEffectManager::~Effekseer3DEffectManager()
{
}

// Effekseer3DEffectManager�̗B��̃C���X�^���X��Ԃ�
Effekseer3DEffectManager& Effekseer3DEffectManager::GetInstance()
{
	// �B��̎���
	static Effekseer3DEffectManager instance;

	// �B��̎��ԎQ�Ƃ�Ԃ�
	return instance;
}

// Effekseer�̏������ƃG�t�F�N�g�̃��[�h
void Effekseer3DEffectManager::Init()
{
//	auto manager = ::Effekseer::Manager::Create(3000);

	// Effekseer������������
	// �����ɂ͉�ʂɕ\������ő�p�[�e�B�N������ݒ肷��
	if (Effkseer_Init(8000) == -1)
	{
		// �������̎��s
		assert(0);
	}

	// �摜�̃��[�h
	imgHandle_ = my::MyLoadGraph("Data/Effect/background.png");

	// �������火��Effect�����[�h����
	LoadEffectFile(EffectID::player_dead.c_str());	
	LoadEffectFile(EffectID::player_boost.c_str());		
	LoadEffectFile(EffectID::player_acceleration.c_str());	
	LoadEffectFile(EffectID::meteor.c_str());		
}

// �X�V
void Effekseer3DEffectManager::Update()
{
	// DX���C�u�����̃J������Effekseer�̃J�����𓯊�����
	Effekseer_Sync3DSetting();

	// Effekseer�ɂ��Đ����̃G�t�F�N�g���X�V����
	UpdateEffekseer3D();
}

// �`��
void Effekseer3DEffectManager::Draw()
{
	// ���ł������̂ŉ摜��`�悷��
	// �������ĕ`�悵����łȂ��ƁAEffekseer�͕`��ł��Ȃ�
	DrawGraph(0, 0, imgHandle_, true);

	// Effekseer�ɂ��Đ����̃G�t�F�N�g��`�悷�� 
	DrawEffekseer3D();
}

// �I������
void Effekseer3DEffectManager::End()
{
	// �摜�̍폜
	DeleteGraph(imgHandle_);

	// �G�t�F�N�g���\�[�X���폜����(Effekseer�I�����ɔj�������̂ō폜���Ȃ��Ă�����)
	for (auto& effectResource : effectResourceHandleTable_)
	{
		DeleteEffekseerEffect(effectResource.second);
	}

	// Effekseer���I������
	Effkseer_End();
}

// �G�t�F�N�g�̃��[�h
void Effekseer3DEffectManager::LoadEffectFile(std::string fileName)
{
	std::string path = data_file_path;
	path += fileName;
	path += data_extension;

	// �G�t�F�N�g�̃��[�h(���s������~�߂�)
	int handle = LoadEffekseerEffect(path.c_str());
	assert(handle != -1);	// -1�ȊO����Ȃ�������~�߂�

	// ���[�h�����G�t�F�N�g�̃n���h���̕ۑ�
	effectResourceHandleTable_[fileName] = handle;
}

// �w��̃G�t�F�N�g�̍Đ�
void Effekseer3DEffectManager::PlayEffect(std::string fileName, bool isForcePlay, VECTOR pos, float scale, float speed, VECTOR rot)
{
	// �G�t�F�N�g���\�[�X�Ɏw�肵���G�t�F�N�g�����[�h����Ă��Ȃ��ꍇ�~�߂�
	assert(effectResourceHandleTable_.find(fileName) != effectResourceHandleTable_.end());

	if (!isForcePlay)
	{
		// �Đ����̏ꍇ�Đ����Ȃ�
		if (IsPlayingEffect(fileName))	return;
	}
	
	// �G�t�F�N�g�̍Đ�(���s������~�߂�)
	playingEffectHandleTable_[fileName] = PlayEffekseer3DEffect(effectResourceHandleTable_[fileName]);
	assert(playingEffectHandleTable_[fileName] != -1); // -1�ȊO����Ȃ�������~�߂�

	// �G�t�F�N�g�̍Đ����x��ݒ�
	SetSpeedPlayingEffekseer3DEffect(playingEffectHandleTable_[fileName], speed);

	// �G�t�F�N�g�̊g�嗦�̐ݒ�
	SetScalePlayingEffekseer3DEffect(playingEffectHandleTable_[fileName], scale, scale, scale);

	// �G�t�F�N�g�̉�]���̐ݒ�
	SetRotationPlayingEffekseer3DEffect(playingEffectHandleTable_[fileName], rot.x, rot.y, rot.z);

	// �G�t�F�N�g�̈ʒu�̐ݒ�
	SetPosPlayingEffekseer3DEffect(playingEffectHandleTable_[fileName], pos.x, pos.y, pos.z);
}

void Effekseer3DEffectManager::SetPosPlayingEffect(std::string fileName, VECTOR pos)
{
	// �G�t�F�N�g���\�[�X�Ɏw�肵���G�t�F�N�g�����[�h����Ă��Ȃ��ꍇ�~�߂�
	assert(playingEffectHandleTable_.find(fileName) != playingEffectHandleTable_.end());

	// �G�t�F�N�g�̈ʒu�̐ݒ�
	SetPosPlayingEffekseer3DEffect(playingEffectHandleTable_[fileName], pos.x, pos.y, pos.z);
}

void Effekseer3DEffectManager::SetScalePlayingEffect(std::string fileName, float scale)
{
	// �G�t�F�N�g���\�[�X�Ɏw�肵���G�t�F�N�g�����[�h����Ă��Ȃ��ꍇ�~�߂�
	assert(playingEffectHandleTable_.find(fileName) != playingEffectHandleTable_.end());

	// �G�t�F�N�g�̊g�嗦�̐ݒ�
	SetScalePlayingEffekseer3DEffect(playingEffectHandleTable_[fileName], scale, scale, scale);
}

void Effekseer3DEffectManager::SetSpeedPlayingEffect(std::string fileName, float speed)
{
	// �G�t�F�N�g���\�[�X�Ɏw�肵���G�t�F�N�g�����[�h����Ă��Ȃ��ꍇ�~�߂�
	assert(playingEffectHandleTable_.find(fileName) != playingEffectHandleTable_.end());

	// �G�t�F�N�g�̍Đ����x��ݒ�
	SetSpeedPlayingEffekseer3DEffect(playingEffectHandleTable_[fileName], speed);
}

void Effekseer3DEffectManager::SetRotPlayingEffect(std::string fileName, VECTOR rot)
{
	// �G�t�F�N�g���\�[�X�Ɏw�肵���G�t�F�N�g�����[�h����Ă��Ȃ��ꍇ�~�߂�
	assert(playingEffectHandleTable_.find(fileName) != playingEffectHandleTable_.end());

	// �G�t�F�N�g�̉�]���̐ݒ�
	SetRotationPlayingEffekseer3DEffect(playingEffectHandleTable_[fileName], rot.x, rot.y, rot.z);
}

// ����̃G�t�F�N�g���Đ�����
bool Effekseer3DEffectManager::IsPlayingEffect(std::string fileName)
{
	if (IsEffekseer3DEffectPlaying(playingEffectHandleTable_[fileName]) == 0)
	{
		return true;
	}
	return false;
}

// ����̃G�t�F�N�g�̍Đ����X�g�b�v
void Effekseer3DEffectManager::StopEffect(std::string fileName)
{
	StopEffekseer3DEffect(playingEffectHandleTable_[fileName]);
}

// �G�t�F�N�g�S�Ă̍Đ����X�g�b�v
void Effekseer3DEffectManager::StopAllEffect()
{
	for (auto& effect : playingEffectHandleTable_)
	{
		StopEffekseer3DEffect(effect.second);
	}
}