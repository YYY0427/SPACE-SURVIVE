#include "DataReaderFromUnity.h"
#include <cassert>

namespace
{
	// // ファイルのパス
	const std::string data_file_path = "Data/ObjectData.dat";

	const std::string player_data_name = "player";
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

		if (data.name == "player")
		{
			playerData_ = data;
		}
		else if (data.name == "enemy")
		{
			enemyData_.push_back(data);
		}
		else
		{
			assert(0);
		}
	}
	FileRead_close(dataHandle);
}

// 度数法から弧度法(180->π)
float DataReaderFromUnity::RadianFromDegree(float degree)
{
	return DX_PI_F * degree / 180.0f;
}

// 読み取ったデータの取得
std::vector<DataReaderFromUnity::UnityGameObject> DataReaderFromUnity::GetEnemyData() const
{
	return enemyData_;
}

DataReaderFromUnity::UnityGameObject DataReaderFromUnity::GetPlayerData() const
{
	return playerData_;
}