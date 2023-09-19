#pragma once
#include <memory>

class Model;

class SkyDome
{
public:
	SkyDome();
	virtual ~SkyDome();

	void Update();
	void Draw();
private:
	std::shared_ptr<Model> pModel_;
};