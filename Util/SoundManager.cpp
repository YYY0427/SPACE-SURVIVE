#include "SoundManager.h"
#include "../Game.h"
#include "SaveData.h"
#include <cassert>
#include <fstream>
#include <sstream>
#include <cassert>

namespace
{
	// サウンドのファイルパス
	const string data_file_path = "Data/Sound/";

	// TODO:多分この関数別のところでも使うからファイル作って移す
	/// <summary>
	/// 文字列を区切る(複数の文字列に変換)
	/// </summary>
	/// <param name="input">区切りたい文字列</param>
	/// <param name="delimiter">文字列を区切る文字</param>
	/// <returns>区切った文字列</returns>
	vector<string> Split(string& input, char delimiter)
	{
		istringstream stream(input);	// stringをgetlineで使えるデータに変換
		string field;					// 分割した文字列1つ分を格納する
		vector<string> result;			// 分割後の文字列の配列
		while (getline(stream, field, delimiter)) 
		{
			result.push_back(field);
		}
		return result;
	}
}

/// <summary>
/// コンストラクタ
/// </summary>
SoundManager::SoundManager()
{
}

/// <summary>
/// デストラクタ
/// </summary>
SoundManager::~SoundManager()
{
}

/// <summary>
/// SoundManagerの唯一のインスタンスを返す
/// </summary>
/// <returns>唯一の実態の参照</returns>
SoundManager& SoundManager::GetInstance()
{
	// 唯一の実態
	static SoundManager instance;

	// 唯一の実態参照を返す
	return instance;
}

/// <summary>
/// 2DSEサウンドのロード
/// </summary>
/// <param name="fileName">ロードしたいサウンドファイル名(拡張子なし)</param>
/// <param name="extension">ロードしたサウンドの拡張子</param>
void SoundManager::LoadSoundFile2D(string fileName, string extension)
{
	// TODO:なんかロードがうまくいかない(-1が返ってくる)から先生に見てもらう
	string path = data_file_path;
	path += fileName;
	path += extension;
	int handle = LoadSoundMem(path.c_str());
	assert(handle != -1);
	soundNameAndHandleTable_[fileName].handle = handle;
}

/// <summary>
/// 3DSEサウンドのロード
/// </summary>
/// <param name="fileName">ロードしたいサウンドファイル名(拡張子なし)</param>
/// <param name="extension">ロードしたサウンドの拡張子</param>
void SoundManager::LoadSoundFile3D(string fileName, string extension)
{
	string path = data_file_path;
	path += fileName;
	path += extension;
	SetCreate3DSoundFlag(TRUE);
	int handle = LoadSoundMem(path.c_str());
	SetCreate3DSoundFlag(FALSE);
	assert(handle != -1);
	soundNameAndHandleTable_[fileName].handle = handle;
}

/// <summary>
/// ファイルからサウンドのデータを読み取ってデータテーブルに格納
/// </summary>
void SoundManager::LoadAndSaveSoundFileData()
{
	// ファイル情報の読み込み(読み込みに失敗したら止める)
	ifstream ifs("Data/Csv/Sound.csv");
	assert(ifs);

	// csvデータを1行ずつ読み取る
	string line;
	while (getline(ifs, line))
	{
		// csvデータ１行を','で複数の文字列に変換
		vector<string>	strvec = Split(line, ',');

		// 文字列を適切なデータ型に変換して格納
		SoundData data;
		data.handle = -1;	// 初期化
		data.volumeRate = stof(strvec[SoundDataType::VOLUM_RATE]);	// string型からfloat型に変換し格納
		data.extension = strvec[SoundDataType::EXTENSION];			// string型で格納
		
		// サウンドタイプの保存
		// 変換したデータをファイル名をキーとして格納
		// サウンドのタイプによってそれぞれロード
		switch (stoi(strvec[SoundDataType::SOUND_TYPE]))
		{
		case SoundType::BGM:
			data.type = SoundType::BGM;
			soundNameAndHandleTable_[strvec[SoundDataType::FILE_NAME]] = data;
			LoadSoundFile2D(strvec[SoundDataType::FILE_NAME], data.extension);
			break;
		case SoundType::SE2D:
			data.type = SoundType::SE2D;
			soundNameAndHandleTable_[strvec[SoundDataType::FILE_NAME]] = data;
			LoadSoundFile2D(strvec[SoundDataType::FILE_NAME], data.extension);
			break;
		case SoundType::SE3D:
			data.type = SoundType::SE3D;
			soundNameAndHandleTable_[strvec[SoundDataType::FILE_NAME]] = data;
			LoadSoundFile3D(strvec[SoundDataType::FILE_NAME], data.extension);
			break;
		default:
			// あり得ない値なので止める
			assert(0);
			break;
		}
	}
}

/// <summary>
/// 指定の2DSEを鳴らす(サウンドをロードされていない場合、2DSE以外の場合は止まる)
/// </summary>
/// <param name="name">再生したいサウンドのファイル名(拡張子は含まない)</param>
void SoundManager::Play(string fileName)
{
	assert(soundNameAndHandleTable_.find(fileName) != soundNameAndHandleTable_.end());	// ロードしていない場合は止める
	assert(soundNameAndHandleTable_[fileName].type == SoundType::SE2D);				// 2DSE以外の場合は止める
	PlaySoundMem(soundNameAndHandleTable_[fileName].handle, DX_PLAYTYPE_BACK);
	SetVolume(fileName, 255);
}

/// <summary>
/// 指定の3DSEを鳴らす(サウンドをロードされていない場合、3DSE以外の場合は止まる)
/// </summary>
/// <param name="fileName">再生したいサウンドのファイル名</param>
/// <param name="soundPos">再生位置</param>
/// <param name="soundRadius">聞こえる距離</param>
void SoundManager::Play3D(string fileName, VECTOR soundPos, float soundRadius)
{
	assert(soundNameAndHandleTable_.find(fileName) != soundNameAndHandleTable_.end());	// ロードしていない場合は止める
	assert(soundNameAndHandleTable_[fileName].type == SoundType::SE3D);				// 3DSE以外の場合は止める
	Set3DPositionSoundMem(soundPos, soundNameAndHandleTable_[fileName].handle);
	Set3DRadiusSoundMem(soundRadius, soundNameAndHandleTable_[fileName].handle);
	PlaySoundMem(soundNameAndHandleTable_[fileName].handle, DX_PLAYTYPE_BACK);
	SetVolume(fileName, 255);
}

/// <summary>
/// 指定のBGMを鳴らす(サウンドをロードされていない場合、BGM以外の場合は止まる)
/// </summary>
/// <param name="fileName">再生したいサウンドのファイル名</param>
void SoundManager::PlayBGM(string fileName)
{
	assert(soundNameAndHandleTable_.find(fileName) != soundNameAndHandleTable_.end());	// ロードしていない場合は止める
	assert(soundNameAndHandleTable_[fileName].type == SoundType::BGM);					// BGM以外の場合は止める
	PlaySoundMem(soundNameAndHandleTable_[fileName].handle, DX_PLAYTYPE_LOOP);
	SetVolume(fileName, 255);
}

/// <summary>
/// 特定のサウンドが再生中かチェック(サウンドがロードされていなかったら止める)
/// </summary>
/// <param name="fileName">再生しているかチェックしたいサウンドのファイル名</param>
/// <returns>true : 再生中、false : 再生していない</returns>
bool SoundManager::PlayingCheckSound(string fileName)
{
	assert(soundNameAndHandleTable_.find(fileName) != soundNameAndHandleTable_.end());	// ロードしていない場合は止める
	bool sound = CheckSoundMem(soundNameAndHandleTable_[fileName].handle);
	return sound;
}

/// <summary>
/// 特定のサウンドを止める(サウンドがロードされていなかったら止める)
/// </summary>
/// <param name="fileName">止めたいサウンドのファイル名(拡張子は含まない)</param>
void SoundManager::StopSound(string fileName)
{
	assert(soundNameAndHandleTable_.find(fileName) != soundNameAndHandleTable_.end());	// ロードしていない場合は止める
	StopSoundMem(soundNameAndHandleTable_[fileName].handle);
}

/// <summary>
/// すべてのサウンドを止める
/// </summary>
void SoundManager::StopAllSound()
{
	for (auto& sound : soundNameAndHandleTable_)
	{
		StopSoundMem(sound.second.handle);
	}
}

void SoundManager::SetVolume(string fileName, int volume)
{
	// サウンドに設定された音量調節
	int setVolume = volume;
	setVolume = static_cast<int>(volume * soundNameAndHandleTable_[fileName].volumeRate);

	// コンフィグで設定した音量調節
	int configVolume = Game::config_volume_num;
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
	float configRate = static_cast<float>(configVolume) / Game::config_volume_num;
	setVolume = static_cast<int>(setVolume * configRate);
	ChangeVolumeSoundMem(setVolume, soundNameAndHandleTable_[fileName].handle);
}

/// <summary>
/// 3Dサウンドのリスナーの位置とリスナーの前方位置を設定する
/// </summary>
/// <param name="pos">リスナーの座標</param>
/// <param name="angle">1リスナーの向いている角度(ラジアン)</param>
void SoundManager::Set3DSoundListenerPosAndFrontPos_UpVecY(VECTOR pos, VECTOR angle)
{
	DxLib::Set3DSoundListenerPosAndFrontPos_UpVecY(pos, VAdd(pos, angle));
}
