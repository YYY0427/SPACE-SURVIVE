#pragma once
#include "../Vector2.h"
#include "../Util/Range.h"
#include <map>
#include <array>

class Warning
{
public:
	Warning();
	~Warning();

	void Update();
	void Draw();
private:
	enum class ID
	{
		CENTER,
		UP,
		DOWN,
	};

private:
	struct Data
	{
		int imgH;
		Vector2 pos;
		int scrollDir;
		int imgWidth;
		int imgHeight;
	};

private:
	std::array<Data, 3> uiDataTable_;
	int alphaParam_;
	int addAlphaValue_;
	Range<int> alphaParamRange_;

	int scroll_;
};