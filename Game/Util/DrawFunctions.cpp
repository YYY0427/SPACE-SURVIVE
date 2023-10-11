#include <DxLib.h>
#include <cassert>
#include "DrawFunctions.h"

namespace my
{
	// ‰æ‘œ‚ğƒ[ƒh‚·‚é
	int MyLoadGraph(const TCHAR* path)
	{
		int handle = LoadGraph(path);

		// -1ˆÈŠO‚¶‚á‚È‚©‚Á‚½‚ç~‚ß‚é
		assert(handle != -1);	

		return handle;
	}
}