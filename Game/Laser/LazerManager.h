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

	void Create(LaserType laserType, VECTOR* pos, VECTOR* vec, VECTOR* fireObjectMoveVec);

	void Update();
	void Draw();

	// Œp‘±ƒŒ[ƒU[‚Ìíœ
	void DeleteContinueLaser();

	const std::list<LaserData>& GetLazeres() const;

private:
	std::list<LaserData> pLaseres_;
	std::unordered_map<LaserType, int> laserModelHanldeTable_;
};