#include "EnergyGage.h"
#include "../Util/DrawFunctions.h"
#include <DxLib.h>
#include <string>

namespace
{
	// エネルギーゲージの拡大率
	constexpr double energy_gagu_scale = 0.3;

	// エネルギーゲージの画像のファイルのパス
	const std::string energy_base_img_file_path = "Data/Image/EnergyBase.png";
	const std::string energy_bar_img_file_path = "Data/Image/EnergyBar.png";
}

// コンストラクタ
EnergyGage::EnergyGage(float maxEnergy) :
	maxEnergy_(maxEnergy)
{
	energyBaseImgH_ = my::MyLoadGraph(energy_base_img_file_path.c_str());
	energyBarImgH_ = my::MyLoadGraph(energy_bar_img_file_path.c_str());
}

// デストラクタ
EnergyGage::~EnergyGage()
{
	DeleteGraph(energyBaseImgH_);
	DeleteGraph(energyBarImgH_);
}

// エネルギーゲージの描画
void EnergyGage::Draw(int x, int y, float energy)
{
	// エネルギーゲージの枠の描画
	DrawRotaGraph(x, y, energy_gagu_scale, 0.0, energyBaseImgH_, true);
	
	// エネルギーがない場合はエネルギーバーの描画を行わない
	if (energy > 0.0f)
	{
		// エネルギーバーの画像のサイズの取得
		float width, height;
		GetGraphSizeF(energyBarImgH_, &width, &height);

		// DrawRectRotaGraphFの性質上、指定の矩形の部分の描画が指定した座標を中心に行われ、画像がずれるため、
		// 画像ずれをなくす割合を計算
		float ratio = (width - (width * (energy / maxEnergy_))) * 0.5f * energy_gagu_scale;

		// エネルギーバーをエネルギーの残量によって描画
		DrawRectRotaGraphF(x - ratio, y, 0, 0, width * (energy / maxEnergy_), height, energy_gagu_scale, 0.0, energyBarImgH_, true);
	}
}
