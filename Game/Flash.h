#pragma once
#include "Vector2.h"
#include "Util/Timer.h"

class Flash
{
public:
	Flash(int enableFrame);
	~Flash();

	void Update(Vector2 pos, unsigned int color);
	void Draw();

	int GetAlpha() const;

	bool IsEnd() const;
private:
	Vector2 pos_;
	Timer<int> enableFrame_;
	unsigned int color_;
	float radius_;
	int alpha_;
};