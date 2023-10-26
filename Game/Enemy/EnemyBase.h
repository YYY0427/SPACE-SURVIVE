#pragma once
#include <memory>

class Model;

class EnemyBase
{
public:
	EnemyBase();
	virtual ~EnemyBase();

	virtual void Update() = 0;
	virtual void Draw() = 0;

protected:
	std::unique_ptr<Model> pModel_;
};