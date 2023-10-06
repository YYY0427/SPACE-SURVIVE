#include "Image3DManager.h"
#include "Image3D.h"
#include "Util/DrawFunctions.h"
#include <string>

namespace
{
	const std::string load_img_file_path = "Data/Image/Road.png";
}

Image3DManager::Image3DManager(std::vector<UnityGameObject> roadsData)
{
	// ‰æ‘œ‚Ìƒ[ƒh
	handleMap_[Image3DType::LOAD] = my::MyLoadGraph(load_img_file_path.c_str());

	for (auto& roadData : roadsData)
	{
		pImgae3Des_.push_back(std::make_shared<Image3D>(handleMap_[Image3DType::LOAD], roadData));
	}
}

Image3DManager::~Image3DManager()
{
	// ‰æ‘œ‚ğ‚·‚×‚Äíœ
	for (auto& handle : handleMap_)
	{
		DeleteGraph(handle.second);
	}
}

void Image3DManager::Draw()
{
	for (auto& image3D : pImgae3Des_)
	{
		image3D->Draw();
	}
}