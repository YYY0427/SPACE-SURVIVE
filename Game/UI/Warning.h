#pragma once
#include "../Vector2.h"
#include "../Util/Range.h"
#include "../Util/Timer.h"
#include <map>
#include <array>

class Warning
{
public:
	Warning(int drawFrame);
	~Warning();

	void Update();
	void Draw();

	bool IsEnd() const;

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

	int stringAlphaParam_;
	int addStringAlphaValue_;
	Range<int> stringAlphaParamRange_;

	int imgAlphaParam_;
	int addImgAlphaValue_;
	Range<int> imgAlphaParamRange_;

	int scroll_;
	Timer drawFrameTimer_;
	bool isEnd_;
};