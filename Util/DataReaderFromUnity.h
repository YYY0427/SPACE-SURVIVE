#pragma once
#include <string>
#include <DxLib.h>
#include <vector>

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
	std::vector<UnityGameObject> GetRockData() const;	// 岩
	std::vector<UnityGameObject> GetMeteorData() const;	// 岩
	UnityGameObject GetPlayerData() const;				// プレイヤー

private:
	// 岩のデータの保存用
	std::vector<UnityGameObject> rockData_;

	// 隕石のデータの保存用
	std::vector<UnityGameObject> meteorData_;

	// プレイヤーのデータの保存用
	UnityGameObject playerData_;
};