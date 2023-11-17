#pragma once
#include "LazerBase.h"
#include <list>
#include <memory>
#include <unordered_map>

struct LazerData
{
	std::shared_ptr<LazerBase> pLazer;
	LazerType type;
};

class LazerManager
{
public:
	LazerManager();
	virtual ~LazerManager();

	void Create(LazerType lazerType, VECTOR* pos, VECTOR* vec, VECTOR* enemyMoveVec);

	void Update();
	void Draw();

	const std::list<LazerData>& GetLazeres() const;
private:
	std::list<LazerData> pLazeres_;
	std::unordered_map<LazerType, int> lazerModelHanldeTable_;
};