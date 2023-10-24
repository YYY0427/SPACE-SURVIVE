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

	void Draw();

	std::shared_ptr<Image3D> GetImage3D() const;
private:
	std::shared_ptr<Image3D> pRoad_;

	VECTOR pos_;
	VECTOR rot_;
};