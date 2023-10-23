#pragma once
#include <DxLib.h>

namespace Debug
{
	// TODO : デバッグログの表示を一番最後に描画するようにする
	void Log(const TCHAR* string);
	void Log(const TCHAR* string, int num);
	void Log(const TCHAR* string, float num);
	void Log(const TCHAR* string, VECTOR num);
	void ClearLog();
}