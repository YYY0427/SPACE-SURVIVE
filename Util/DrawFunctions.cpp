#include <DxLib.h>
#include <cassert>
#include "DrawFunctions.h"

namespace my
{
	/// <summary>
	/// 画像をロードする
	/// </summary>
	/// <param name="path">パス文字列</param>
	/// <returns>画像のハンドル</returns>
	int MyLoadGraph(const TCHAR* path)
	{
		int handle = LoadGraph(path);
		assert(handle != -1);	// -1以外じゃなかったら止める
		return handle;
	}
}