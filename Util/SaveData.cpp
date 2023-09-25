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
			saveData_ = data;
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
	assert(saveData_.version == current_save_version);
	std::ofstream ofs(data_file_path, std::ios_base::binary);
	ofs.write(reinterpret_cast<char*>(&saveData_), sizeof(Data));
}

// セーブデータを新規作成して上書き
void SaveData::CreateNewData()
{
	// セーブデータの初期化
	InitData();

	std::ofstream ofs(data_file_path, std::ios_base::binary);
	ofs.write(reinterpret_cast<char*>(&saveData_), sizeof(SaveData));
}

// セーブデータの初期化
void SaveData::InitData()
{
	saveData_.version = current_save_version;
	saveData_.bgmVolume = 5;
	saveData_.seVolume = 5;
	saveData_.wholeVolume = 5;
	saveData_.padStickSensitivityX = 6;
	saveData_.padStickSensitivityY = 3;
	saveData_.padStickReverseX = false;	
	saveData_.padStickReverseY = false;	
}

// BGMの音量の取得
int SaveData::GetBgmVolume() const
{
	return saveData_.bgmVolume;
}

// SEの音量の取得
int SaveData::GetSeVolume() const
{
	return saveData_.seVolume;
}

// パッドのスティックのX軸感度の取得
int SaveData::GetPadStickSensitivityX() const
{
	return saveData_.padStickSensitivityX;
}

// パッドのスティックのY軸感度の取得
int SaveData::GetPadStickSensitivityY() const
{
	return saveData_.padStickSensitivityY;
}

// パッドのスティックのX軸リバースの取得
bool SaveData::GetPadStickReverseX() const
{
	return saveData_.padStickReverseX;
}

// パッドのスティックのY軸リバースの取得
bool SaveData::GetPadStickReverseY() const
{
	return saveData_.padStickReverseY;
}

template<class T> void SaveData::SetConfigValue(T* configValue, int splitNum)
{
	configValue++;
	if (configValue > splitNum)
	{
		configValue = 0;
	}
	// セーブデータに書き込む
	SaveData::GetInstance().Write();
}

// 全体の音量の設定
// 最大値を超えると0に戻る
void SaveData::SetWholeVolume()
{
	saveData_.wholeVolume++;
	if (saveData_.wholeVolume > common::config_volume_num)
	{
		saveData_.wholeVolume = 0;
	}
	// セーブデータに書き込む
	SaveData::GetInstance().Write();
}

// BGMの音量の設定
// 最大値を超えると0に戻る
void SaveData::SetBgmVolume()
{
	saveData_.bgmVolume++;
	if (saveData_.bgmVolume > common::config_volume_num)
	{
		saveData_.bgmVolume = 0;
	}
	// セーブデータに書き込む
	SaveData::GetInstance().Write();
}

// SEの音量の設定
// 最大値を超えると0に戻る
void SaveData::SetSeVolume()
{
	saveData_.seVolume++;
	if (saveData_.seVolume > common::config_volume_num)
	{
		saveData_.seVolume = 0;
	}

	// セーブデータに書き込む
	SaveData::GetInstance().Write();
}

// パッドのスティックのX軸感度の設定
// 最大値を超えると0に戻る
void SaveData::SetPadStickSensitivityX()
{
	saveData_.padStickSensitivityX++;
	if (saveData_.padStickSensitivityX > common::config_pad_sensitivity_num)
	{
		saveData_.padStickSensitivityX = 0;
	}
	// セーブデータに書き込む
	SaveData::GetInstance().Write();
}

// パッドのスティックのY軸感度の設定
// 最大値を超えると0に戻る
void SaveData::SetPadStickSensitivityY()
{
	saveData_.padStickSensitivityY++;
	if (saveData_.padStickSensitivityY > common::config_pad_sensitivity_num)
	{
		saveData_.padStickSensitivityY = 0;
	}
	// セーブデータに書き込む
	SaveData::GetInstance().Write();
}

// パッドのスティックのX軸リバースの設定
void SaveData::SetPadStickReverseX()
{
	if (saveData_.padStickReverseX)
	{
		saveData_.padStickReverseX = false;
	}
	else
	{
		saveData_.padStickReverseX = true;
	}
	// セーブデータに書き込む
	SaveData::GetInstance().Write();
}

// パッドのスティックのY軸リバースの設定
void SaveData::SetPadStickReverseY()
{
	if (saveData_.padStickReverseY)
	{
		saveData_.padStickReverseY = false;
	}
	else
	{
		saveData_.padStickReverseY = true;
	}
	// セーブデータに書き込む
	SaveData::GetInstance().Write();
}

SaveData::Data SaveData::GetSaveData()
{
	return saveData_;
}