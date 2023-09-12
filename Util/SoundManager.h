#pragma once
#include <unordered_map>
#include <string>
#include <DxLib.h>

using namespace std;

/// <summary>
/// サウンドの管理を行うシングルトンクラス
/// </summary>
class SoundManager
{
public:
	// デストラクタ
	~SoundManager();

	// SoundManagerの唯一のインスタンスを返す
	static SoundManager& GetInstance();

	// ファイルからサウンドのデータを読み取ってデータテーブルに格納
	void LoadAndSaveSoundFileData();

	// 指定の2DSEを鳴らす
	void Play(string fileName);

	// 指定の3DSEを鳴らす
	void Play3D(string fileName, VECTOR soundPos, float soundRadius);

	// 指定のBGMを鳴らす
	void PlayBGM(string fileName);

	// 指定のサウンドが再生中かチェック
	bool PlayingCheckSound(string fileName);

	// 特定のサウンドを止める
	void StopSound(string fileName);

	// すべてのサウンドを止める
	void StopAllSound();

	void SetVolume(string fileName, int volume);

	// 3Dサウンドのリスナーの位置とリスナーの前方位置を設定する
	void Set3DSoundListenerPosAndFrontPos_UpVecY(VECTOR pos, VECTOR angle);
private:
	// サウンドの種類
	enum SoundType : int
	{
		BGM,
		SE2D,
		SE3D,
	};
	// サウンドのデータの種類
	enum SoundDataType : int
	{
		FILE_NAME,
		EXTENSION,
		SOUND_TYPE,
		VOLUM_RATE,
	};
private:
	// サウンドのデータ
	struct SoundData
	{
		SoundType type;		// BGMか3DのSEか2DのSEか
		string extension;	// サウンドファイルの拡張子
		float volumeRate;	// ボリューム調整
		int handle;			// ハンドル
	};
private:
	// コンストラクタ(シングルトンパターンなのでprivateに置く)
	SoundManager();

	// コピーも代入も禁止する
	SoundManager(const SoundManager&) = delete;			// コピーコンストラクタ
	void operator = (const SoundManager&) = delete;		// 代入も禁止

	// 2DSEサウンドのロード
	void LoadSoundFile2D(string fileName, string ext);

	// 3DSEサウンドのロード
	void LoadSoundFile3D(string fileName, string ext);
private:
	// ロードしたサウンドのファイル名とハンドル
	unordered_map<string, SoundData> soundNameAndHandleTable_;
};