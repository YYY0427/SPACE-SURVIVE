#include "SaveData.h"
#include "SoundManager.h"
#include "Range.h"
#include "InputState.h"
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
SaveData::SaveData() :
	soundManger_(SoundManager::GetInstance())
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
	saveData_.masterVolume = 5;
	saveData_.padStickSensitivityX = 6;
	saveData_.padStickSensitivityY = 3;
	saveData_.padStickReverseX = false;	
	saveData_.padStickReverseY = false;	
	saveData_.windowMode = false;
}

template<class T> void SaveData::SetConfigValue(T& configValue, int splitNum)
{
	if (InputState::IsTriggered(InputType::RIGHT))
	{
		configValue++;
	}
	if (InputState::IsTriggered(InputType::LEFT))
	{
		configValue--;
	}

	const Range<T> configValueRange(0, splitNum + 1);

	configValue = configValueRange.Wrap(configValue);

	// セーブデータに書き込む
	SaveData::GetInstance().Write();
}

// 全体の音量の設定
// 最大値を超えると0に戻る
void SaveData::SetMasterVolume()
{
	SetConfigValue(saveData_.masterVolume, common::config_volume_num);

	if (InputState::IsTriggered(InputType::RIGHT) ||
		InputState::IsTriggered(InputType::LEFT))
	{
		soundManger_.SetVolume("bgmTest", 255);
	}
}

// BGMの音量の設定
// 最大値を超えると0に戻る
void SaveData::SetBgmVolume()
{
	SetConfigValue(saveData_.bgmVolume, common::config_volume_num);

	if (InputState::IsTriggered(InputType::RIGHT) ||
		InputState::IsTriggered(InputType::LEFT))
	{
		soundManger_.SetVolume("bgmTest", 255);
	}
}

// SEの音量の設定
// 最大値を超えると0に戻る
void SaveData::SetSeVolume()
{
	SetConfigValue(saveData_.seVolume, common::config_volume_num);

	if (InputState::IsTriggered(InputType::RIGHT) ||
		InputState::IsTriggered(InputType::LEFT))
	{
		soundManger_.Play("cursorTest");
	}
}

// パッドのスティックのX軸感度の設定
// 最大値を超えると0に戻る
void SaveData::SetPadStickSensitivityX()
{
	SetConfigValue(saveData_.padStickSensitivityX, common::config_pad_sensitivity_num);
}

// パッドのスティックのY軸感度の設定
// 最大値を超えると0に戻る
void SaveData::SetPadStickSensitivityY()
{
	SetConfigValue(saveData_.padStickSensitivityY, common::config_pad_sensitivity_num);
}

// パッドのスティックのX軸リバースの設定
void SaveData::SetPadStickReverseX()
{
	SetConfigSwitch(saveData_.padStickReverseX);
}

// パッドのスティックのY軸リバースの設定
void SaveData::SetPadStickReverseY()
{
	SetConfigSwitch(saveData_.padStickReverseY);
}

void SaveData::SetWindowMode()
{
	SetConfigSwitch(saveData_.windowMode);
}

SaveData::Data SaveData::GetSaveData() const
{
	return saveData_;
}

void SaveData::SetConfigSwitch(bool& config)
{
	if (InputState::IsTriggered(InputType::RIGHT) ||
		InputState::IsTriggered(InputType::LEFT))
	{
		if (config)
		{
			config = false;
		}
		else
		{
			config = true;
		}
	}

	// セーブデータに書き込む
	SaveData::GetInstance().Write();
}
