#include <DxLib.h>
#include <cassert>
#include "DrawFunctions.h"

namespace my
{
	// �摜�����[�h����
	int MyLoadGraph(const TCHAR* path)
	{
		int handle = LoadGraph(path);

		// -1�ȊO����Ȃ�������~�߂�
		assert(handle != -1);	

		return handle;
	}
}