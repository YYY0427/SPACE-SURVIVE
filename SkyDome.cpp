#include "SkyDome.h"
#include "Util/Model.h"
#include <string>

namespace
{
	const std::string model_file_path = "Data/Model/testSkydome.mv1";

	constexpr float model_scale = 5.0f;
}

SkyDome::SkyDome()
{
	pModel_ = std::make_shared<Model>(model_file_path);
	pModel_->SetPos(VGet(0, 0, 0));
	pModel_->SetScale(VGet(model_scale, model_scale, model_scale));
}

SkyDome::~SkyDome()
{
}

void SkyDome::Update()
{
	pModel_->Update();
}

void SkyDome::Draw()
{
	pModel_->Draw();
}