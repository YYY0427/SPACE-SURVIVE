#pragma once
#include "Vector2.h"
#include <vector>

class UIBase
{
public:
	UIBase();
	virtual ~UIBase();

	// UI‚ÌŠi”[
	virtual void Store(Vector2 vec, float speed);

private:
	Vector2 pos_;
};