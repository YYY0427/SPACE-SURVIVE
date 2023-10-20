#include "Debug.h"
#include <string>

namespace
{
	int y_ = 0;

	constexpr int draw_width = 0;
	constexpr int string_space = 16;
	constexpr unsigned int string_color = 0xffffff;
}

namespace Debug
{
	void Log(const TCHAR* string)
	{
#ifdef _DEBUG
		DrawString(draw_width, y_, string, string_color);
		y_ += string_space;
#endif
	}

	void Log(const TCHAR* string, int num)
	{
#ifdef _DEBUG
		int width = GetDrawStringWidth(string, static_cast<int>(strlen(string)));
		DrawString(draw_width, y_, string, string_color);
		DrawExtendFormatString(draw_width + width, y_, 1.0, 1.0, string_color, " = %d", num);
		y_ += string_space;
#endif
	};

	void Log(const TCHAR* string, float num)
	{
#ifdef _DEBUG
		int width = GetDrawStringWidth(string, static_cast<int>(strlen(string)));
		DrawString(draw_width, y_, string, string_color);
		DrawExtendFormatString(draw_width + width, y_, 1.0, 1.0, string_color, " = %f", num);
		y_ += string_space;
#endif
	};

	void Log(const TCHAR* string, VECTOR num)
	{
#ifdef _DEBUG
		int width = GetDrawStringWidth(string, static_cast<int>(strlen(string)));
		DrawString(draw_width, y_, string, string_color);
		DrawExtendFormatString(draw_width + width, y_, 1.0, 1.0, string_color, " = %f, %f, %f", num.x, num.y, num.z);
		y_ += string_space;
#endif
	}

	void ClearLog()
	{
		y_ = 0;
	};
}