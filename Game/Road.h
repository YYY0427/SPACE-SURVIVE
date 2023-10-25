#pragma once
#include <DxLib.h>
#include <memory>
#include "Util/DataReaderFromUnity.h"
#include "Image3D.h"

class Road
{
public:
	Road(UnityGameObject data);
	virtual ~Road();

	void Update(VECTOR playerPos);
	void Draw();

	VECTOR GetPos() const;
	int GetImageWidth() const;
	int GetImageHeight() const;
	int GetImageHandle() const;
	std::array<VERTEX3D, 6> GetVertex() const;

	void SetPos(VECTOR pos);

private:
	std::shared_ptr<Image3D> pRoad_;

	VECTOR pos_;
	VECTOR rot_;
	int imageWidth_;
	int imageHeight_;
};