#pragma once
#include <string>
#include <DxLib.h>
#include <vector>

// Unityで配置したオブジェクトのデータを読み取るクラス
class DataReaderFromUnity
{
private:
	// 
	struct UnityGameObject
	{
		std::string name;
		VECTOR pos;
		VECTOR rot;
	};
public:
	// コンストラクタ
	DataReaderFromUnity();

	// デストラクタ
	virtual ~DataReaderFromUnity();

	// Unityで配置したオブジェクトのデータを読み取る
	void LoadUnityGameObjectData();

	/// <summary>
	/// 度数法から弧度法(180->π)
	/// </summary>
	/// <param name="degree">変換したい角度</param>
	/// <returns>変換後の角度</returns>
	float RadianFromDegree(float degree);

	// 読み取ったデータの取得
	std::vector<UnityGameObject> GetData() const;
private:
	// 
	std::vector<UnityGameObject> data_;
};