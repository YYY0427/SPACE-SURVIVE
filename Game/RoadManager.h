#pragma once
#include <map>
#include <memory>
#include <vector>
#include "Util/DataReaderFromUnity.h"

// プロトタイプ宣言
class Road;

// 3D画像の管理クラス
class RoadManager
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="roadsData">Unityから読み込んだ道のデータのテーブル</param>
	RoadManager(std::vector<UnityGameObject> roadsData);

	// デストラクタ
	virtual ~RoadManager();

	// 描画
	void Draw();

	// 道の取得
	std::vector<std::shared_ptr<Road>> GetRoads() const;

	// Y軸成分を除いた全ての道の中から受けっとった位置情報に1番近い道の座標の取得
	VECTOR GetClosestRoadPos(VECTOR targetPos);

private:
	// 3D画像の種別
	enum class Image3DType
	{
		ROAD,
	};
private:
	std::vector<std::shared_ptr<Road>> pRoads_;
};