#pragma once
#include <DxLib.h>

namespace Debug
{
	// TODO : �f�o�b�O���O�̕\������ԍŌ�ɕ`�悷��悤�ɂ���
	void Log(const TCHAR* string);
	void Log(const TCHAR* string, int num);
	void Log(const TCHAR* string, float num);
	void Log(const TCHAR* string, VECTOR num);
	void ClearLog();
	void DrawLog();
}