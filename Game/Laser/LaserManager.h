#pragma once
#include "LaserBase.h"
#include <list>
#include <memory>
#include <unordered_map>

struct LaserData
{
	std::shared_ptr<LaserBase> pLaser;
	LaserType type;
};

class LaserManager
{
public:
	LaserManager();
	~LaserManager();

	void Create(LaserType laserType, VECTOR* pos, VECTOR* vec, int fireFrameCount = 1.0f, int chareFrameTime = 100);

	void Update();
	void Draw();

	/// <summary>
	/// レーザーを反射
	/// </summary>
	/// <param name="pos">反射するレーザーを発射する位置</param>
	/// <param name="norm">シールドの法線情報</param>
	void Reflect(const VECTOR pos, const VECTOR vec, const VECTOR normal);

	// 反射レーザーの削除
	void DeleteReflectLaser();

	// 継続レーザーの削除
	void DeleteContinueLaser();

	// 全てのレーザーの削除
	void GraduallyAlphaDeleteAllLaser();

	// レーザーの取得
	const std::list<LaserData>& GetLazeres() const;

private:
	std::list<LaserData> pLaseres_;
	std::unordered_map<LaserType, int> laserModelHanldeTable_;
};