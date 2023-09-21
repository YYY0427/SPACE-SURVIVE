#include "SaveData.h"
#include "../common.h"
#include <string>
#include <cassert>
#include <fstream>
#include <DxLib.h>

namespace
{
	// セーブデータファイル名
	const std::string data_file_path = "Data/Save/Data.dat";

	// セーブデータバージョン　
	constexpr int current_save_version = 0;
}

// コンストラクタ
SaveData::SaveData()
{

}

// デストラクタ
SaveData::~SaveData()
{
}

// 唯一のインスタンスを返す
SaveData& SaveData::GetInstance()
{
	// 唯一の実態
	static SaveData instance;

	// 唯一の実態参照を返す
	return instance;
}

// セーブデータの読み込み
void SaveData::Load()
{
	// セーブデータの初期化
	InitData();

	std::ifstream ifs;
	ifs.open(data_file_path, std::ios_base::binary);

	// ファイル読み込み失敗
	if (ifs.fail())
	{
		// セーブデータを作る
		CreateNewData();
		return;
	}
	else
	{
		// セーブデータの読み込み
		Data data;
		ifs.read(reinterpret_cast<char*>(&data), sizeof(Data));
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

// セーブデータの書き込み
void SaveData::Write()
{
	assert(data_.version == current_save_version);
	std::ofstream ofs(data_file_path, std::ios_base::binary);
	ofs.write(reinterpret_cast<char*>(&data_), sizeof(Data));
}

// セーブデータを新規作成して上書き
void SaveData::CreateNewData()
{
	// セーブデータの初期化
	InitData();

	std::ofstream ofs(data_file_path, std::ios_base::binary);
	ofs.write(reinterpret_cast<char*>(&data_), sizeof(SaveData));
}

// セーブデータの初期化
void SaveData::InitData()
{
	data_.version = current_save_version;
	data_.volumeBgm = 3;
	data_.volumeSe = 3;
	data_.padStickSensitivityX = 5;
	data_.padStickSensitivityY = 3;
	data_.padStickReverseX = false;	
	data_.padStickReverseY = false;	
}

// BGMの音量の取得
int SaveData::GetBgmVolume() const
{
	return data_.volumeBgm;
}

// SEの音量の取得
int SaveData::GetSeVolume() const
{
	return data_.volumeSe;
}

// パッドのスティックのX軸感度の取得
int SaveData::GetPadStickSensitivityX() const
{
	return data_.padStickSensitivityX;
}

// パッドのスティックのY軸感度の取得
int SaveData::GetPadStickSensitivityY() const
{
	return data_.padStickSensitivityY;
}

// パッドのスティックのX軸リバースの取得
bool SaveData::GetPadStickReverseX() const
{
	return data_.padStickReverseX;
}

// パッドのスティックのY軸リバースの取得
bool SaveData::GetPadStickReverseY() const
{
	return data_.padStickReverseY;
}

// BGMの音量の設定
// 最大値を超えると0に戻る
void SaveData::SetBgmVolume()
{
	data_.volumeBgm++;
	if (data_.volumeBgm > common::config_volume_num)
	{
		data_.volumeBgm = 0;
	}

	// セーブデータに書き込む
	SaveData::GetInstance().Write();
}

// SEの音量の設定
// 最大値を超えると0に戻る
void SaveData::SetSeVolume()
{
	data_.volumeSe++;
	if (data_.volumeSe > common::config_volume_num)
	{
		data_.volumeSe = 0;
	}

	// セーブデータに書き込む
	SaveData::GetInstance().Write();
}

// パッドのスティックのX軸感度の設定
// 最大値を超えると0に戻る
void SaveData::SetPadStickSensitivityX()
{
	data_.padStickSensitivityX++;
	if (data_.padStickSensitivityX > 10)
	{
		data_.padStickSensitivityX = 0;
	}
	// セーブデータに書き込む
	SaveData::GetInstance().Write();
}

// パッドのスティックのY軸感度の設定
// 最大値を超えると0に戻る
void SaveData::SetPadStickSensitivityY()
{
	data_.padStickSensitivityY++;
	if (data_.padStickSensitivityY > 10)
	{
		data_.padStickSensitivityY = 0;
	}
	// セーブデータに書き込む
	SaveData::GetInstance().Write();
}

// パッドのスティックのX軸リバースの設定
void SaveData::SetPadStickReverseX()
{
	if (data_.padStickReverseX)
	{
		data_.padStickReverseX = false;
	}
	else
	{
		data_.padStickReverseX = true;
	}
	// セーブデータに書き込む
	SaveData::GetInstance().Write();
}

// パッドのスティックのY軸リバースの設定
void SaveData::SetPadStickReverseY()
{
	if (data_.padStickReverseY)
	{
		data_.padStickReverseY = false;
	}
	else
	{
		data_.padStickReverseY = true;
	}
	// セーブデータに書き込む
	SaveData::GetInstance().Write();
}