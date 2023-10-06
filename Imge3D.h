#pragma once
#include <DxLib.h>
class Imge3D
{
public:
	Imge3D(int imgHandle, VECTOR pos, VECTOR rot);
	virtual ~Imge3D();

	void Draw();
private:
	VERTEX3D vectex_[6];
	VECTOR pos_;
	VECTOR rot_;
	int imgHandle_;
};