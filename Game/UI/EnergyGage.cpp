#include "EnergyGage.h"
#include "../Util/DrawFunctions.h"
#include <DxLib.h>
#include <string>

namespace
{
	constexpr double energy_gagu_scale = 0.4;
	const std::string energy_base_img_file_path = "Data/Image/EnergyBase.png";
	const std::string energy_bar_img_file_path = "Data/Image/EnergyBar.png";
}

EnergyGage::EnergyGage(float maxEnergy) :
	maxEnergy_(maxEnergy)
{
	energyBaseImgH_ = my::MyLoadGraph(energy_base_img_file_path.c_str());
	energyBarImgH_ = my::MyLoadGraph(energy_bar_img_file_path.c_str());
}

EnergyGage::~EnergyGage()
{
}

void EnergyGage::Update()
{
}

void EnergyGage::Draw(int x, int y, float energy)
{
	DrawRotaGraph(x, y, energy_gagu_scale, 0.0, energyBaseImgH_, true);

	float width, height;
	GetGraphSizeF(energyBarImgH_ ,&width, &height);

	if (energy > 0.0f)
	{
	//	DrawRectGraphF(x - (width / 2), y - (height / 2), 0, 0, width * (energy / maxEnergy_), height, /*energy_gagu_scale, 0.0, */energyBarImgH_, true);
		float a = width - (width * (energy / maxEnergy_));
		DrawRectRotaGraphF(x - a * 0.5 * energy_gagu_scale, y, 0, 0, width * (energy / maxEnergy_), height, energy_gagu_scale, 0.0, energyBarImgH_, true);
	}
	
}
