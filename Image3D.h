#pragma once
#include <DxLib.h>
#include "Util/DataReaderFromUnity.h"

class Image3D
{
public:
	Image3D(int imgHandle, UnityGameObject data);
	virtual ~Image3D();

	void Draw();
private:
	VERTEX3D vectex_[6];
	int imgHandle_;
};