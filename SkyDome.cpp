#include "SkyDome.h"
#include "Player.h"
#include "Util/Model.h"
#include <string>

namespace
{
	const std::string model_file_path = "Data/Model/MV1/SkyDome.mv1";

	constexpr float model_scale = 2000.0f;
}

SkyDome::SkyDome(std::shared_ptr<Player> pPlayer) : 
	pPlayer_(pPlayer)
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
	pModel_->SetPos(pPlayer_->GetPos());
	pModel_->Update();
}

void SkyDome::Draw()
{
	pModel_->Draw();
}