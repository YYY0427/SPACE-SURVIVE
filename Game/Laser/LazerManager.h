#pragma once
#include "LazerBase.h"
#include <list>
#include <memory>
#include <unordered_map>

struct LaserData
{
	std::shared_ptr<LazerBase> pLaser;
	LaserType type;
};

class LazerManager
{
public:
	LazerManager();
	~LazerManager();

	void Create(LaserType laserType, VECTOR* pos, VECTOR* vec);

	void Update();
	void Draw();

	/// <summary>
	/// レーザーを反射
	/// </summary>
	/// <param name="pos">反射するレーザーを発射する位置</param>
	/// <param name="norm">シールドの法線情報</param>
	void Reflect(const VECTOR pos, const VECTOR vec, const VECTOR normal);

	void DeleteReflectLaser();

	// 継続レーザーの削除
	void DeleteContinueLaser();

	const std::list<LaserData>& GetLazeres() const;

private:
	std::list<LaserData> pLaseres_;
	std::unordered_map<LaserType, int> laserModelHanldeTable_;

	VECTOR reflectLaserPos_;
	VECTOR reflectVec_;
};