#pragma once
#include <string>
#include <DxLib.h>
#include <vector>
#include <unordered_map>

struct UnityGameObject
{
	std::string name;
	VECTOR pos;
	VECTOR rot;
	VECTOR scale;
};

// Unityで配置したオブジェクトのデータを読み取るクラス
class DataReaderFromUnity
{
public:
	// デストラクタ
	~DataReaderFromUnity();

	/// <summary>
	/// 唯一のインスタンスを返す
	/// </summary>
	/// <returns>唯一の実態の参照</returns>
	static DataReaderFromUnity& GetInstance();

	// Unityで配置したオブジェクトのデータを読み取る
	void LoadUnityGameObjectData(const TCHAR* fileName);

	/// <summary>
	/// 度数法から弧度法(180->π)
	/// </summary>
	/// <param name="degree">変換したい角度</param>
	/// <returns>変換後の角度</returns>
	float RadianFromDegree(float degree);

	// 読み取ったデータの取得
	std::vector<UnityGameObject> GetData(std::string objectName) const;

private:
	// コンストラクタ
	DataReaderFromUnity();

	// コピーも代入も禁止
	DataReaderFromUnity(const DataReaderFromUnity&) = delete;	// コピーコンストラクタ禁止
	void operator = (const DataReaderFromUnity&) = delete;		// 代入禁止

	// Unityでのオブジェクトの名前のIDとしたUnityで読み取ったデータのデータタイプのテーブル
	std::unordered_map<std::string, std::vector<UnityGameObject>> data_;
};