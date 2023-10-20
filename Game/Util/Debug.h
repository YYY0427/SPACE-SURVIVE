#pragma once
#include <DxLib.h>

namespace Debug
{
	void Log(const TCHAR* string);
	void Log(const TCHAR* string, int num);
	void Log(const TCHAR* string, float num);
	void Log(const TCHAR* string, VECTOR num);
	void ClearLog();
}