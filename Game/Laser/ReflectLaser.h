#pragma once
#include "LazerBase.h"

class ReflectLaser : public LazerBase
{
public:
	ReflectLaser();
	~ReflectLaser();

	void Update() override;
	void Draw() override;

private:

};