#pragma once
#include <DxLib.h>
#include <array>
#include "Util/DataReaderFromUnity.h"

class Image3D
{
public:
	Image3D(int imgHandle, UnityGameObject data);
	virtual ~Image3D();

	void Draw();

	VECTOR GetPos() const;
	VECTOR GetRot() const;
	float GetImgWidth() const;
	float GetImgHeight() const;
	int GetImgHandle() const;
	std::array<VERTEX3D, 6> GetVertex() const;
private:
	std::array<VERTEX3D, 6> vectex_;
	VECTOR pos_;
	VECTOR rot_;
	int imgHandle_;
	float imgWidth_;
	float imgHeight_;
};