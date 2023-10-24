#include "Road.h"
#include "Image3D.h"
#include "Util/DrawFunctions.h"
#include <string>

namespace
{
	// 画像のファイルパス
	const std::string img_file_path = "Data/Image/Road.png";
}

Road::Road(UnityGameObject data)
{
	pRoad_ = std::make_shared<Image3D>(img_file_path);

	pRoad_->SetPos(data.pos);
	pRoad_->SetRot({ data.rot.x + (90.0f * DX_PI_F / 180.0f), data.rot.y, data.rot.z });
	pRoad_->SetImgWidth(500.0f * data.scale.x);
	pRoad_->SetImgHeight(500.0f * data.scale.z);
	pRoad_->Update();
}

Road::~Road()
{
}

void Road::Draw()
{
	pRoad_->Draw();
}

std::shared_ptr<Image3D> Road::GetImage3D() const
{
	return pRoad_;
}