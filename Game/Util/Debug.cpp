#include "Debug.h"
#include <string>
#include <map>
#include <list>

namespace
{
	std::list<std::string> logOnlyTable_;
	template <typename T> std::map<std::string, T> logTable_;

	int y_ = 0;

	constexpr int draw_width = 0;
	constexpr int string_space = 16;
	constexpr unsigned int string_color = 0xffffff;
}

namespace Debug
{
	void Log(const TCHAR* string)
	{
		logOnlyTable_.push_back(string);
	}

	void Log(const TCHAR* string, int num)
	{
		logTable_<int>[string] = num;
	};

	void Log(const TCHAR* string, float num)
	{
		logTable_<float>[string] = num;
	};

	void Log(const TCHAR* string, VECTOR num)
	{
		logTable_<VECTOR>[string] = num;
	}

	void ClearLog()
	{
		y_ = 0;
		logOnlyTable_.clear();
		logTable_<int>.clear();
		logTable_<float>.clear();
		logTable_<VECTOR>.clear();
	}

	void DrawLog()
	{
#ifdef _DEBUG
		for (auto& log : logOnlyTable_)
		{
			DrawString(draw_width, y_, log.c_str(), string_color);
			y_ += string_space;
		}
		for (auto& log : logTable_<int>)
		{
			int width = GetDrawStringWidth(log.first.c_str(), static_cast<int>(strlen(log.first.c_str())));
			DrawString(draw_width, y_, log.first.c_str(), string_color);
			DrawExtendFormatString(draw_width + width, y_, 1.0, 1.0, string_color, " = %d", log.second);
			y_ += string_space;
		}
		for (auto& log : logTable_<float>)
		{
			int width = GetDrawStringWidth(log.first.c_str(), static_cast<int>(strlen(log.first.c_str())));
			DrawString(draw_width, y_, log.first.c_str(), string_color);
			DrawExtendFormatString(draw_width + width, y_, 1.0, 1.0, string_color, " = %f", log.second);
			y_ += string_space;
		}
		for (auto& log : logTable_<VECTOR>)
		{
			int width = GetDrawStringWidth(log.first.c_str(), static_cast<int>(strlen(log.first.c_str())));
			DrawString(draw_width, y_, log.first.c_str(), string_color);
			DrawExtendFormatString(draw_width + width, y_, 1.0, 1.0, string_color, " = %f, %f, %f", log.second.x, log.second.y, log.second.z);
			y_ += string_space; 
		}
#endif
	}
}