#include "RoadManager.h"
#include "Road.h"
#include "Util/DrawFunctions.h"
#include <string>

// コンストラクタ
RoadManager::RoadManager(std::vector<UnityGameObject> roadsData)
{
	// 受けっとったデータの数インスタンスの作成
	for (auto& roadData : roadsData)
	{
		pRoads_.push_back(std::make_shared<Road>(roadData));
	}
}

// デストラクタ
RoadManager::~RoadManager()
{
}

void RoadManager::Update(VECTOR playerPos)
{
	for (auto& road : pRoads_)
	{
		road->Update(playerPos);
	}
}

// 描画
void RoadManager::Draw()
{
	// 道の描画
	for (auto& road : pRoads_)
	{
		road->Draw();
	}
}

// 全ての道の取得
std::vector<std::shared_ptr<Road>> RoadManager::GetRoads() const
{
	return pRoads_;
}

// Y軸成分を除いた全ての道の中から受けっとった位置情報に1番近い道の座標の取得
VECTOR RoadManager::GetClosestRoadPos(VECTOR targetPos)
{
	// 1番ターゲットまで近い道
	VECTOR closestPos = VGet(0.0f, 0.0f, 0.0f);

	// ターゲットまで1番近い大きさ
	float nearDistanceSize = 0.0f;

	// 1回だけ通すようの変数
	bool isFirst = false;

	for (auto& road : pRoads_)
	{
		// Y軸成分を除く
		targetPos = VGet(targetPos.x, 0.0f, targetPos.z);
		VECTOR roadPos = VGet(road->GetPos().x, 0.0f, road->GetPos().z);

		// 道からターゲットまでの距離の大きさを求める
		float distanceSize = VSize(VSub(roadPos, targetPos));

		// ターゲットまで1番近い大きさと現在の道からターゲットまでの距離の大きさを比べる
		// 現在の道の距離ほうか小さかったら情報の更新
		// 1番最初の道は比べる大きさがないので通す
		if (nearDistanceSize > distanceSize || !isFirst)
		{
			// 1回通ったのでフラグを立てる
			isFirst = true;

			// 1番ターゲットまで近い大きさの更新
			nearDistanceSize = distanceSize;

			// 1番ターゲットまで近い道の更新
			closestPos = VGet(road->GetPos().x, road->GetPos().y, road->GetPos().z);
		}
	}
	return closestPos;
}