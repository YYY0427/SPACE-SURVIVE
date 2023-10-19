#include "Effekseer3DEffectManager.h"
#include "EffekseerForDXLib.h"
#include "DrawFunctions.h"
#include <cassert>



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
	// ���[�v�G�t�F�N�g�̏ꍇ�̏���
	for (auto& effect : effectDataTable_)
	{
		// �Đ����Ă��Ȃ��Ƃ������ʂ�Ȃ�
		if (IsPlayingEffect(*effect.playingEffectHandle)) continue;
		if (effect.type == PlayType::LOOP || effect.type == PlayType::LOOP_AND_FOLLOW)
		{
			PlayEffectLoop(effect.playingEffectHandle, effect.effectFileName);
		}
	}
	// �Ǐ]�G�t�F�N�g�̏ꍇ�̏���
	for (auto& effect : effectDataTable_)
	{
		// �Đ��������ʂ�Ȃ�
		if (!IsPlayingEffect(*effect.playingEffectHandle)) continue;
		if (effect.type == PlayType::FOLLOW || effect.type == PlayType::LOOP_AND_FOLLOW)
		{
			SetEffectParam(*effect.playingEffectHandle, *effect.pos, *effect.scale, *effect.speed, *effect.rot);
		}
	}
	// �Đ����I�������G�t�F�N�g���������ꍇ�폜
	effectDataTable_.remove_if([this](EffectData data) { return !IsPlayingEffect(*data.playingEffectHandle); });

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
	// �G�t�F�N�g�̃t�@�C���p�X�̐���
	std::string path = data_file_path;
	path += fileName;
	path += data_extension;

	// �G�t�F�N�g�̃��[�h(���s������~�߂�)
	int handle = LoadEffekseerEffect(path.c_str());
	assert(handle != -1);	// -1�ȊO����Ȃ�������~�߂�

	// ���[�h�����G�t�F�N�g�̃n���h���̕ۑ�
	effectResourceHandleTable_[fileName] = handle;
}

// ���[�v�G�t�F�N�g�̍Đ�
void Effekseer3DEffectManager::PlayEffectLoop(int* playingEffectHandle, std::string fileName)
{
	// �G�t�F�N�g���\�[�X�Ɏw�肵���G�t�F�N�g�����[�h����Ă��Ȃ��ꍇ�~�߂�
	assert(effectResourceHandleTable_.find(fileName) != effectResourceHandleTable_.end());

	// �G�t�F�N�g�̍čĐ�
	*playingEffectHandle = PlayEffekseer3DEffect(effectResourceHandleTable_[fileName]);
}

// �w��̃G�t�F�N�g�̍Đ�
void Effekseer3DEffectManager::PlayEffect(int* playingEffectHandle, std::string fileName, PlayType type, VECTOR* pos, float* scale, float* speed, VECTOR* rot)
{
	// �G�t�F�N�g���\�[�X�Ɏw�肵���G�t�F�N�g�����[�h����Ă��Ȃ��ꍇ�~�߂�
	assert(effectResourceHandleTable_.find(fileName) != effectResourceHandleTable_.end());

	EffectData data{};
	data.effectFileName = fileName;
	*playingEffectHandle = PlayEffekseer3DEffect(effectResourceHandleTable_[fileName]);
	data.playingEffectHandle = playingEffectHandle;
	data.type = type;
	data.pos = pos;
	data.rot = rot;
	data.scale = scale;
	data.speed = speed;
	effectDataTable_.push_back(data);

	SetEffectParam(*data.playingEffectHandle, *data.pos, *data.scale, *data.speed, *data.rot);
}

void Effekseer3DEffectManager::SetEffectParam(int playingEffectHandle, VECTOR pos, float scale, float speed, VECTOR rot)
{
	// �G�t�F�N�g�̍Đ����x��ݒ�
	SetSpeedPlayingEffekseer3DEffect(playingEffectHandle, speed);

	// �G�t�F�N�g�̊g�嗦�̐ݒ�
	SetScalePlayingEffekseer3DEffect(playingEffectHandle, scale, scale, scale);

	// �G�t�F�N�g�̉�]���̐ݒ�
	SetRotationPlayingEffekseer3DEffect(playingEffectHandle, rot.x, rot.y, rot.z);

	// �G�t�F�N�g�̈ʒu�̐ݒ�
	SetPosPlayingEffekseer3DEffect(playingEffectHandle, pos.x, pos.y, pos.z);
}

// ����̃G�t�F�N�g���Đ�����
bool Effekseer3DEffectManager::IsPlayingEffect(int effectPlayingHandle)
{
	if (IsEffekseer3DEffectPlaying(effectPlayingHandle) == 0)
	{
		return true;
	}
	return false;
}

void Effekseer3DEffectManager::StopEffect(int effectPlayingHandle)
{
	int result = StopEffekseer3DEffect(effectPlayingHandle);
	assert(result != -1);
}

// �G�t�F�N�g�S�Ă̍Đ����X�g�b�v
void Effekseer3DEffectManager::StopAllEffect()
{
	for (auto& effect : effectDataTable_)
	{
		StopEffekseer3DEffect(*effect.playingEffectHandle);
	}
}

void Effekseer3DEffectManager::DeleteEffect(int effectPlayingHandle)
{
	StopEffect(effectPlayingHandle);
	effectDataTable_.remove_if([effectPlayingHandle](EffectData data) { return *data.playingEffectHandle == effectPlayingHandle; });
}

void Effekseer3DEffectManager::DeleteAllEffect()
{
	StopAllEffect();
	effectDataTable_.clear();
}