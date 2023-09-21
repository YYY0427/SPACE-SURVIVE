#include "SoundManager.h"
#include "../common.h"
#include "SaveData.h"
#include <cassert>
#include <fstream>
#include <sstream>
#include <cassert>

namespace
{
	// サウンドのファイルパス
	const std::string data_file_path = "Data/Sound/";

	// TODO:多分この関数別のところでも使うからファイル作って移す
	/// <summary>
	/// 文字列を区切る(複数の文字列に変換)
	/// </summary>
	/// <param name="input">区切りたい文字列</param>
	/// <param name="delimiter">文字列を区切る文字</param>
	/// <returns>区切った文字列</returns>
	std::vector<std::string> SplitString(std::string& input, char delimiter)
	{
		std::istringstream stream(input);	// stringをgetlineで使えるデータに変換
		std::string field;					// 分割した文字列1つ分を格納する
		std::vector<std::string> result;	// 分割後の文字列の配列
		while (getline(stream, field, delimiter)) 
		{
			result.push_back(field);
		}
		return result;
	}
}

// コンストラクタ
SoundManager::SoundManager()
{
}

// デストラクタ
SoundManager::~SoundManager()
{
}

// 唯一のインスタンスを返す
SoundManager& SoundManager::GetInstance()
{
	// 唯一の実態
	static SoundManager instance;

	// 唯一の実態参照を返す
	return instance;
}

// 2Dサウンドのロード
void SoundManager::LoadSoundFile2D(std::string fileName, std::string extension)
{
	std::string path = data_file_path;
	path += fileName;
	path += extension;
	int handle = LoadSoundMem(path.c_str());
	assert(handle != -1);
	soundNameAndHandleTable_[fileName].handle = handle;
}

// 3Dサウンドのロード
void SoundManager::LoadSoundFile3D(std::string fileName, std::string extension)
{
	std::string path = data_file_path;
	path += fileName;
	path += extension;
	SetCreate3DSoundFlag(TRUE);
	int handle = LoadSoundMem(path.c_str());
	SetCreate3DSoundFlag(FALSE);
	assert(handle != -1);
	soundNameAndHandleTable_[fileName].handle = handle;
}

// ファイルからサウンドのデータを読み取ってデータテーブルに格納
void SoundManager::LoadAndSaveSoundFileData()
{
	// ファイル情報の読み込み(読み込みに失敗したら止める)
	std::ifstream ifs("Data/Csv/Sound.csv");
	assert(ifs);

	// csvデータを1行ずつ読み取る
	std::string line;
	while (getline(ifs, line))
	{
		// csvデータ１行を','で複数の文字列に変換
		std::vector<std::string> strvec = SplitString(line, ',');

		// 文字列を適切なデータ型に変換して格納
		SoundData data;
		data.handle = -1;	// 初期化
		data.volumeRate = std::stof(strvec[static_cast<int>(SoundDataType::VOLUM_RATE)]);	// string型からfloat型に変換し格納
		data.extension = strvec[static_cast<int>(SoundDataType::EXTENSION)];				// string型で格納
		
		// サウンドタイプの保存
		// 変換したデータをファイル名をキーとして格納
		// サウンドのタイプによってそれぞれロード
		int iSoundType = std::stoi(strvec[static_cast<int>(SoundDataType::SOUND_TYPE)]);	// string型からint型に変換
		SoundType soundType = static_cast<SoundType>(iSoundType);							// int型からSoundType型に変換
		switch (soundType)
		{
		case SoundType::BGM:
			data.type = SoundType::BGM;
			soundNameAndHandleTable_[strvec[static_cast<int>(SoundDataType::FILE_NAME)]] = data;
			LoadSoundFile2D(strvec[static_cast<int>(SoundDataType::FILE_NAME)], data.extension);
			break;
		case SoundType::SE2D:
			data.type = SoundType::SE2D;
			soundNameAndHandleTable_[strvec[static_cast<int>(SoundDataType::FILE_NAME)]] = data;
			LoadSoundFile2D(strvec[static_cast<int>(SoundDataType::FILE_NAME)], data.extension);
			break;
		case SoundType::SE3D:
			data.type = SoundType::SE3D;
			soundNameAndHandleTable_[strvec[static_cast<int>(SoundDataType::FILE_NAME)]] = data;
			LoadSoundFile3D(strvec[static_cast<int>(SoundDataType::FILE_NAME)], data.extension);
			break;
		default:
			// あり得ない値なので止める
			assert(0);
			break;
		}
	}
}

// 指定の2DSEを鳴らす
void SoundManager::Play(std::string fileName)
{
	assert(soundNameAndHandleTable_.find(fileName) != soundNameAndHandleTable_.end());	// ロードしていない場合は止める
	assert(soundNameAndHandleTable_[fileName].type == SoundType::SE2D);					// 2DSE以外の場合は止める
	PlaySoundMem(soundNameAndHandleTable_[fileName].handle, DX_PLAYTYPE_BACK);
	SetVolume(fileName, 255);
}

// 指定の3DSEを鳴らす
void SoundManager::Play3D(std::string fileName, VECTOR soundPos, float soundRadius)
{
	assert(soundNameAndHandleTable_.find(fileName) != soundNameAndHandleTable_.end());	// ロードしていない場合は止める
	assert(soundNameAndHandleTable_[fileName].type == SoundType::SE3D);					// 3DSE以外の場合は止める
	Set3DPositionSoundMem(soundPos, soundNameAndHandleTable_[fileName].handle);
	Set3DRadiusSoundMem(soundRadius, soundNameAndHandleTable_[fileName].handle);
	PlaySoundMem(soundNameAndHandleTable_[fileName].handle, DX_PLAYTYPE_BACK);
	SetVolume(fileName, 255);
}

// 指定のBGMを鳴らす
void SoundManager::PlayBGM(std::string fileName)
{
	assert(soundNameAndHandleTable_.find(fileName) != soundNameAndHandleTable_.end());	// ロードしていない場合は止める
	assert(soundNameAndHandleTable_[fileName].type == SoundType::BGM);					// BGM以外の場合は止める
	PlaySoundMem(soundNameAndHandleTable_[fileName].handle, DX_PLAYTYPE_LOOP);
	SetVolume(fileName, 255);
}

// 特定のサウンドが再生中かチェック
bool SoundManager::PlayingCheckSound(std::string fileName)
{
	assert(soundNameAndHandleTable_.find(fileName) != soundNameAndHandleTable_.end());	// ロードしていない場合は止める
	bool sound = CheckSoundMem(soundNameAndHandleTable_[fileName].handle);
	return sound;
}

// 特定のサウンドを止める
void SoundManager::StopSound(std::string fileName)
{
	assert(soundNameAndHandleTable_.find(fileName) != soundNameAndHandleTable_.end());	// ロードしていない場合は止める
	StopSoundMem(soundNameAndHandleTable_[fileName].handle);
}

// すべてのサウンドを止める
void SoundManager::StopAllSound()
{
	for (auto& sound : soundNameAndHandleTable_)
	{
		StopSoundMem(sound.second.handle);
	}
}

// 音量調節
void SoundManager::SetVolume(std::string fileName, int volume)
{
	// サウンドに設定された音量調節
	int setVolume = volume;
	setVolume = static_cast<int>(volume * soundNameAndHandleTable_[fileName].volumeRate);

	// コンフィグで設定した音量調節
	int configVolume = common::config_volume_num;

	// BGM
	if (soundNameAndHandleTable_[fileName].type == SoundType::BGM)
	{
		configVolume = SaveData::GetInstance().GetBgmVolume();
	}
	// SE
	else
	{
		configVolume = SaveData::GetInstance().GetSeVolume();
	}

	// 設定したい音量とサウンドに設定された音量とコンフィグで設定された音量から求めた最終的な音量に設定
	float configRate = static_cast<float>(configVolume) / common::config_volume_num;
	setVolume = static_cast<int>(setVolume * configRate);
	ChangeVolumeSoundMem(setVolume, soundNameAndHandleTable_[fileName].handle);
}

// 3Dサウンドのリスナーの位置とリスナーの前方位置を設定する
void SoundManager::Set3DSoundListenerPosAndFrontPos_UpVecY(VECTOR pos, VECTOR angle)
{
	DxLib::Set3DSoundListenerPosAndFrontPos_UpVecY(pos, VAdd(pos, angle));
}
