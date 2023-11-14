#pragma once
#include "../Vector2.h"
#include "../Util/Range.h"
#include <map>

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
	};

private:
	enum class ID
	{
		CENTER,
		DOWN,
		UP,
		TOTAL_VALUE
	};

private:
	std::map<ID, Data> uiDataTable_;
	int alphaParam_;
	int nameDousiyou;
	Range<int> alphaParamRange_;
};