#pragma once
#include "LazerBase.h"
#include <list>
#include <memory>
#include <unordered_map>

class LazerManager
{
public:
	LazerManager();
	virtual ~LazerManager();

	void Create(LazerType lazerType, const VECTOR pos, const VECTOR vec);

	void Update();
	void Draw();

	const std::list<std::unique_ptr<LazerBase>>& GetLazeres() const;
private:
	std::list<std::unique_ptr<LazerBase>> pLazeres_;
	std::unordered_map<LazerType, int> lazerModelHanldeTable_;
};