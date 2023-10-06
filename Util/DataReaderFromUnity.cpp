#include "DataReaderFromUnity.h"
#include <cassert>

namespace
{
	// ファイルのパス
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

		// Unityとの座標データを合わせるために100倍
		data.pos = VScale(data.pos, 100);

		// 回転データxyzを読む
		result = FileRead_read(&data.rot, sizeof(data.rot), dataHandle);
		assert(result != -1);

		if (data.name == "Stage")
		{
			data.rot.x = data.rot.x + 90.0f;
		}

		// 度数法を弧度法に変換
		data.rot.x = RadianFromDegree(data.rot.x);
		data.rot.y = RadianFromDegree(data.rot.y);
		data.rot.z = RadianFromDegree(data.rot.z);

		// 拡大データxyzを読む
		result = FileRead_read(&data.scale, sizeof(data.scale), dataHandle);
		assert(result != -1);
		
		// オブジェクトの名前ごとに保存
		data_[data.name].push_back(data);
	}

	// ファイルを閉じる
	FileRead_close(dataHandle);
}

// 度数法から弧度法(180->π)
float DataReaderFromUnity::RadianFromDegree(float degree)
{
	return DX_PI_F * degree / 180.0f;
}

// 読み取ったデータの取得
std::unordered_map<std::string, std::vector<UnityGameObject>> DataReaderFromUnity::GetData() const
{
	return data_;
}
