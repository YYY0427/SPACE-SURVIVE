#include "PlanetManager.h"
#include "PlanetBase.h"
#include <string>
#include <DxLib.h>
#include <cassert>
#include "Sun.h"
#include "Earth.h"
#include "../Util/DrawFunctions.h"

namespace
{
	const std::string model_file_hierarchy = "Data/Model/MV1/";
	const std::string model_file_extension = ".mv1";

	const std::string sun_model_file_name = "Sun";
	const std::string earth_model_file_name = "Earth";
}

PlanetManager::PlanetManager()
{
	// ÉÇÉfÉãÇÃÉçÅ[Éh
	std::string sunFilePath = model_file_hierarchy + sun_model_file_name + model_file_extension;
	handleTable_[PlanetType::SUN] = my::MyLoadModel(sunFilePath.c_str());

	std::string earthFilePath = model_file_hierarchy + earth_model_file_name + model_file_extension;
	handleTable_[PlanetType::EARTH] = my::MyLoadModel(earthFilePath.c_str());

//	pPlanets_.push_back(std::make_shared<Sun>(handleTable_[PlanetType::SUN], sunData.front()));
//	pPlanets_.push_back(std::make_shared<Earth>(handleTable_[PlanetType::EARTH], earthData.front()));
}

PlanetManager::~PlanetManager()
{
	for (auto& handle : handleTable_)
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

const std::list<std::shared_ptr<PlanetBase>>& PlanetManager::GetPlanets() const
{
	return pPlanets_;
}
