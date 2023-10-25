#include "Road.h"
#include "Image3D.h"
#include "Util/DrawFunctions.h"
#include "Util/Debug.h"
#include <string>

namespace
{
	// 画像のファイルパス
	const std::string img_file_path = "Data/Image/Road.png";
}

Road::Road(UnityGameObject data) :
	pos_(data.pos),
	rot_({ data.rot.x + (90.0f * DX_PI_F / 180.0f), data.rot.y, data.rot.z }),
	imageWidth_(500.0f * data.scale.x),
	imageHeight_(500.0f * data.scale.z)
{
	pRoad_ = std::make_shared<Image3D>(img_file_path);

	pRoad_->SetPos(pos_);
	pRoad_->SetRot(rot_);
	pRoad_->SetImgWidth(imageWidth_);
	pRoad_->SetImgHeight(imageHeight_);
	pRoad_->Update();
}

Road::~Road()
{
}


void Road::Update(VECTOR playerPos)
{
	pos_.z -= 20.0f;

	

	pRoad_->SetPos(pos_);
	pRoad_->SetRot(rot_);
	pRoad_->Update();
}

void Road::Draw()
{
	pRoad_->Draw();
}

VECTOR Road::GetPos() const
{
	return pos_;
}

int Road::GetImageWidth() const
{
	return imageWidth_;
}

int Road::GetImageHeight() const
{
	return imageHeight_;
}

int Road::GetImageHandle() const
{
	return pRoad_->GetImageHandle();
}

std::array<VERTEX3D, 6> Road::GetVertex() const
{
	return pRoad_->GetVertex();
}

void Road::SetPos(VECTOR pos)
{
	pos_ = pos;
}
