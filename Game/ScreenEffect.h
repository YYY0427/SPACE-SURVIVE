#pragma once
#include "Util/Timer.h"

class ScreenEffect
{
public:
	ScreenEffect();
	~ScreenEffect();

	void Update();
	void Draw(int screenHandle);

	void SetShake(float quakeXSize, float quakeYSize, int frame);

private:
	Timer<int> frame_;
	float quakeX_;
	float quakeY_;
};