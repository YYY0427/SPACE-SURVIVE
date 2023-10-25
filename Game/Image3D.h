#pragma once
#include <string>
#include <array>
#include <DxLib.h>

class Image3D
{
public:
	Image3D(std::string filePath);
	Image3D(int imgHandle);

	virtual ~Image3D();

	void Update();
	void Draw();

	int GetImageHandle() const;
	std::array<VERTEX3D, 6> GetVertex() const;

	void SetPos(VECTOR pos);
	void SetRot(VECTOR rot);
	void SetImgWidth(float width);
	void SetImgHeight(float height);
	void SetImgDafualtScale();

private:
	std::array<VERTEX3D, 6> vectex_;
	VECTOR pos_;
	VECTOR rot_;
	int imgHandle_;
	float imgWidth_;
	float imgHeight_;
};