#pragma once
#include "Vector2.h"
#include <vector>

class UIBase
{
public:
	UIBase();
	virtual ~UIBase();

	virtual void Draw() {};

	// UI‚ÌŠi”[
	void Store(Vector2 vec);


	bool IsEnabled() const;

protected:
	Vector2 pos_;
	bool isEnabled_;
};