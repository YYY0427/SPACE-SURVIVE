#pragma once
#include "LazerBase.h"
#include <list>
#include <memory>
#include <unordered_map>

struct LaserData
{
	std::shared_ptr<LazerBase> pLazer;
	LaserType type;
};

class LazerManager
{
public:
	LazerManager();
	virtual ~LazerManager();

	void Create(LaserType laserType, VECTOR* pos, VECTOR* vec, VECTOR* fireObjectMoveVec);

	void Update();
	void Draw();

	const std::list<LaserData>& GetLazeres() const;
private:
	std::list<LaserData> pLazeres_;
	std::unordered_map<LaserType, int> laserModelHanldeTable_;
};