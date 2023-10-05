#include "PlanetManager.h"
#include "PlanetBase.h"
#include <string>
#include <DxLib.h>
#include <cassert>
#include "Sun.h"
#include "Earth.h"

namespace
{
	const std::string sun_model_file_path = "Data/Model/MV1/Sun.mv1";
	const std::string earth_model_file_path = "Data/Model/MV1/Earth.mv1";
}

PlanetManager::PlanetManager(std::vector<UnityGameObject> sunData, std::vector<UnityGameObject> earthData)
{
	// モデルのロード
	handleMap_[PlanetType::SUN] = MV1LoadModel(sun_model_file_path.c_str());
	handleMap_[PlanetType::EARTH] = MV1LoadModel(earth_model_file_path.c_str());

	// 1つでもモデルのロードに失敗したら止める
	for (auto& handle : handleMap_)
	{
		assert(handle.second != -1);
	}

	pPlanets_.push_back(std::make_shared<Sun>(handleMap_[PlanetType::SUN], sunData.front()));
	pPlanets_.push_back(std::make_shared<Earth>(handleMap_[PlanetType::EARTH], earthData.front()));
}

PlanetManager::~PlanetManager()
{
	for (auto& handle : handleMap_)
	{
		MV1DeleteModel(handle.second);
	}
}

void PlanetManager::Update()
{
	for (auto& planet : pPlanets_)
	{
		planet->Update();
	}
}

void PlanetManager::Draw()
{
	for (auto& planet : pPlanets_)
	{
		planet->Draw();
	}
}

std::list<std::shared_ptr<PlanetBase>> PlanetManager::GetPlanets() const
{
	return pPlanets_;
}
