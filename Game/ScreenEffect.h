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
	void SetScreenColor(unsigned int color, int alpha, int speed);

private:
	// ‰æ–Ê—h‚ê
	Timer<int> quakeFrame_;
	float quakeX_;
	float quakeY_;

	// ‰æ–Ê‚ÌF
	unsigned int color_;
	int alpha_;
	int speed_;
};