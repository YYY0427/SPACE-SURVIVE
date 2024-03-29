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
	// 画面揺れ
	Timer<int> quakeFrame_;
	float quakeX_;
	float quakeY_;

	// 画面の色
	unsigned int color_;
	int alpha_;
	int speed_;
};