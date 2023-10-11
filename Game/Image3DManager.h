#pragma once
#include <map>
#include <memory>
#include <vector>
#include "Util/DataReaderFromUnity.h"

// プロトタイプ宣言
class Image3D;

// 3D画像の管理クラス
class Image3DManager
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="roadsData">Unityから読み込んだ道のデータのテーブル</param>
	Image3DManager(std::vector<UnityGameObject> roadsData);

	// デストラクタ
	virtual ~Image3DManager();

	// 描画
	void Draw();

	// 全ての道の取得
	std::vector<std::shared_ptr<Image3D>> GetRoads() const;

	// Y軸成分を除いた全ての道の中から受けっとった位置情報に1番近い道の座標の取得
	VECTOR GetClosestRoadPos(VECTOR targetPos);

private:
	// 3D画像の種別
	enum class Image3DType
	{
		LOAD,
	};
private:
	std::vector<std::shared_ptr<Image3D>> pRoads_;

	// 画像のハンドルを格納するテーブル
	std::map<Image3DType, int> handleTable_;
};