#include "SaveData.h"
#include <string>
#include <cassert>
#include <fstream>
#include <DxLib.h>

using namespace std;

namespace
{
	// セーブデータファイル名
	const string data_file_path = "Data/Save/Data.dat";

	// セーブデータバージョン　
	constexpr int current_save_version = 0;
}

/// <summary>
/// コンストラクタ
/// </summary>
SaveData::SaveData()
{

}

/// <summary>
/// デストラクタ
/// </summary>
SaveData::~SaveData()
{
}

/// <summary>
/// SaveDataの唯一のインスタンスを返す
/// </summary>
/// <returns>唯一の実態の参照</returns>
SaveData& SaveData::GetInstance()
{
	// 唯一の実態
	static SaveData instance;

	// 唯一の実態参照を返す
	return instance;
}

/// <summary>
/// セーブデータの読み込み
/// </summary>
void SaveData::Load()
{
	// セーブデータの初期化
	InitData();

	std::ifstream ifs;
	ifs.open(data_file_path, std::ios_base::binary);
	if (ifs.fail())
	{
		// ファイル読み込み失敗 セーブデータを作る
		CreateNewData();
		return;
	}
	else
	{
		Data data;
		ifs.read((char*)&data, sizeof(Data));
		ifs.close();

		if (data.version == current_save_version)
		{
			// 読み込んだデータを実際に使用するデータにコピー
			data_ = data;
		}
		else
		{
			// リリース後にセーブデータのバージョンが変わったら実装
			assert(0);
		}
	}
}

/// <summary>
/// セーブデータの書き込み
/// </summary>
void SaveData::Write()
{
	assert(data_.version == current_save_version);
	std::ofstream ofs(data_file_path, std::ios_base::binary);
	ofs.write((char*)&data_, sizeof(Data));
}

/// <summary>
/// セーブデータを新規作成して上書き
/// </summary>
void SaveData::CreateNewData()
{
	// セーブデータの初期化
	InitData();

	std::ofstream ofs(data_file_path, std::ios_base::binary);
	ofs.write((char*)&data_, sizeof(SaveData));
}

/// <summary>
/// セーブデータの初期化
/// </summary>
void SaveData::InitData()
{
	data_.version = current_save_version;
	data_.volumeBgm = 3;
	data_.volumeSe = 3;
}

/// <summary>
/// BGMの音量の取得(0~5)
/// </summary>
/// <returns>BGMの音量(0~5)</returns>
int SaveData::GetBgmVolume() const
{
	return data_.volumeBgm;
}

/// <summary>
/// SEの音量の取得(0~5)
/// </summary>
/// <returns>SEの音量(0~5)</returns>
int SaveData::GetSeVolume() const
{
	return data_.volumeSe;
}