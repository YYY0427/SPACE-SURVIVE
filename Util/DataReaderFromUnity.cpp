#include "DataReaderFromUnity.h"
#include <cassert>

namespace
{
	// ファイルのパス
	const std::string data_file_path = "Data/ObjectData.dat";

	const std::string player_data_name = "Player";
	const std::string rock_data_name = "Rock";
	const std::string meteor_data_name = "Meteor";
}

// コンストラクタ
DataReaderFromUnity::DataReaderFromUnity() 
{
}

// デストラクタ
DataReaderFromUnity::~DataReaderFromUnity()
{
}

// Unityで配置したオブジェクトのデータを読み取る
void DataReaderFromUnity::LoadUnityGameObjectData()
{
	// ファイルにアクセス
	auto dataHandle = FileRead_open(data_file_path.c_str());

	// データ数を得る
	int dataNum = 0;
	int result = FileRead_read(&dataNum, sizeof(dataNum), dataHandle);
	assert(result != -1);

	// データの読み取り
	for (int i = 0; i < dataNum; i++)
	{
		UnityGameObject data;

		// 名前の文字列数の読み取り
		uint8_t nameSize = 0;
		result = FileRead_read(&nameSize, sizeof(nameSize), dataHandle);
		assert(result != -1);

		// 名前そのものを読み取る
		data.name.resize(nameSize);
		result = FileRead_read(data.name.data(), sizeof(char) * nameSize, dataHandle);
		assert(result != -1);

		// 座標データxyzを読む
		result = FileRead_read(&data.pos, sizeof(data.pos), dataHandle);
		assert(result != -1);

		// 回転データxyzを読む
		result = FileRead_read(&data.rot, sizeof(data.rot), dataHandle);
		assert(result != -1);

		// 度数法を弧度法に変換
		data.rot.x = RadianFromDegree(data.rot.x);
		data.rot.y = RadianFromDegree(data.rot.y);
		data.rot.z = RadianFromDegree(data.rot.z);

		// 名前によって保存する変数を変える
		if (data.name == player_data_name)
		{
			playerData_ = data;
		}
		else if (data.name == rock_data_name)
		{
			rockData_.push_back(data);
		}
		else if (data.name == meteor_data_name)
		{
			meteorData_.push_back(data);
		}
		else
		{
			// ありえないので止める
			assert(0);
		}
	}
	// ファイルを閉じる
	FileRead_close(dataHandle);
}

// 度数法から弧度法(180->π)
float DataReaderFromUnity::RadianFromDegree(float degree)
{
	return DX_PI_F * degree / 180.0f;
}

// 岩のデータの取得
std::vector<DataReaderFromUnity::UnityGameObject> DataReaderFromUnity::GetRockData() const
{
	return rockData_;
}

// 隕石のデータの取得
std::vector<DataReaderFromUnity::UnityGameObject> DataReaderFromUnity::GetMeteorData() const
{
	return meteorData_;
}

// プレイヤーのデータの取得
DataReaderFromUnity::UnityGameObject DataReaderFromUnity::GetPlayerData() const
{
	return playerData_;
}