#include "Warning.h"
#include "../Util/DrawFunctions.h"
#include "../Util/StringManager.h"
#include "Common.h"
#include <string>
#include <DxLib.h>

namespace
{
	// 画像のファイルパス
	const std::string center_img_file_path = "Data/Image/Warning.png";
	const std::string up_img_file_path = "Data/Image/WarningUp.png";
	const std::string down_img_file_path = "Data/Image/WarningDown.png";
}

Warning::Warning() :
	alphaParam_(0),
	nameDousiyou_(1),
	alphaParamRange_(30, 255)
{
	// 画像のロード
	uiDataTable_[ID::CENTER].imgH_ = my::MyLoadGraph(center_img_file_path.c_str());
	uiDataTable_[ID::UP].imgH_ = my::MyLoadGraph(up_img_file_path.c_str());
	uiDataTable_[ID::DOWN].imgH_ = my::MyLoadGraph(down_img_file_path.c_str());

	// 座標の初期化
	uiDataTable_[ID::CENTER].pos_.x_ = static_cast<float>(common::screen_width) / 2;
	uiDataTable_[ID::CENTER].pos_.y_ = static_cast<float>(common::screen_height) / 2;
	uiDataTable_[ID::UP].pos_.x_ = static_cast<float>(common::screen_width) / 2;
	uiDataTable_[ID::UP].pos_.y_ = static_cast<float>((common::screen_height / 2) - 200);
	uiDataTable_[ID::DOWN].pos_.x_ = static_cast<float>(common::screen_width) / 2;
	uiDataTable_[ID::DOWN].pos_.y_ = static_cast<float>((common::screen_height / 2) + 200);
}

Warning::~Warning()
{
	for (auto& data : uiDataTable_)
	{
		DeleteGraph(data.second.imgH_);
	}
}

void Warning::Update()
{
	alphaParam_ += nameDousiyou_;
	if (alphaParamRange_.IsInside(alphaParam_))
	{
		nameDousiyou_ *= -1;
		alphaParamRange_.Clamp(alphaParam_);
	}

	uiDataTable_[ID::UP].pos_.x_++;
	uiDataTable_[ID::DOWN].pos_.x_--;
}

void Warning::Draw()
{
	for (auto& data : uiDataTable_)
	{
		DrawRotaGraph(data.second.pos_.x_, data.second.pos_.y_, 1.0f, 0.0f, data.second.imgH_, true);
	}

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, alphaParam_);
	StringManager::GetInstance().DrawStringCenter("WarningUI", common::screen_width / 2, common::screen_height / 2, 0xff0000);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}
