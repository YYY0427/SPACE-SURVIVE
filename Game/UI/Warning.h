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
	struct Data
	{
		int imgH_;
		Vector2 pos_;
		int imgWidth;
		int imgHeight_;
	};

private:
	enum class ID
	{
		CENTER,
		DOWN,
		UP
	};

private:
	std::map<ID, std::array<Data, 2>> uiDataTable_;
	int alphaParam_;
	int addAlphaValue_;
	Range<int> alphaParamRange_;
};