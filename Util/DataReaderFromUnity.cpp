#include "DataReaderFromUnity.h"
#include <cassert>

namespace
{
	// // ファイルのパス
	const std::string data_file_path = "Data/ObjectData.dat";
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

	// オブジェクト数ぶん配列を確保
	data_.resize(dataNum);

	// データの読み取り
	for (int i = 0; i < dataNum; i++)
	{
		auto& data = data_[i];

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
	}
	FileRead_close(dataHandle);

	// 度数法を弧度法に変換
	for (auto& object : data_)
	{
		object.rot.x = RadianFromDegree(object.rot.x);
		object.rot.y = RadianFromDegree(object.rot.y);
		object.rot.z = RadianFromDegree(object.rot.z);
	}
}

// 度数法から弧度法(180->π)
float DataReaderFromUnity::RadianFromDegree(float degree)
{
	return DX_PI_F * degree / 180.0f;
}

// 読み取ったデータの取得
std::vector<DataReaderFromUnity::UnityGameObject> DataReaderFromUnity::GetData() const
{
	return data_;
}