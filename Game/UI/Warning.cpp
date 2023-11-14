#include "Warning.h"
#include "../Util/DrawFunctions.h"
#include "../Util/StringManager.h"
#include "Common.h"
#include "../Util/Debug.h"
#include <string>
#include <DxLib.h>

namespace
{
	// 画像のファイルパス
	const std::string center_img_file_path = "Data/Image/Warning.png";
	const std::string up_img_file_path = "Data/Image/WarningUp.png";
	const std::string down_img_file_path = "Data/Image/WarningDown.png";

	// 文字のフェードの速度
	constexpr int string_fade_speed = 5;
}

Warning::Warning() :
	alphaParam_(255),
	addAlphaValue_(string_fade_speed),
	alphaParamRange_(10, 255)
{
	// 画像のロード
	uiDataTable_[ID::CENTER][0].imgH_ = my::MyLoadGraph(center_img_file_path.c_str());
	uiDataTable_[ID::CENTER][1].imgH_ = -1;

	uiDataTable_[ID::UP][0].imgH_ = my::MyLoadGraph(up_img_file_path.c_str());
	uiDataTable_[ID::UP][1].imgH_ = uiDataTable_[ID::UP][0].imgH_;
	uiDataTable_[ID::DOWN][0].imgH_ = my::MyLoadGraph(down_img_file_path.c_str());
	uiDataTable_[ID::DOWN][1].imgH_ = uiDataTable_[ID::DOWN][0].imgH_;

	// グラフィックのサイズの取得
	for (auto& data : uiDataTable_)
	{
		GetGraphSize(data.second[0].imgH_, &data.second[0].imgWidth, &data.second[0].imgHeight_);
	}

	// 座標の初期化
	uiDataTable_[ID::CENTER][0].pos_.x_ = static_cast<float>(common::screen_width) / 2;
	uiDataTable_[ID::CENTER][0].pos_.y_ = static_cast<float>(common::screen_height) / 2;

	uiDataTable_[ID::UP][0].pos_.x_ = static_cast<float>(common::screen_width) / 2;
	uiDataTable_[ID::UP][0].pos_.y_ = static_cast<float>((common::screen_height / 2) - 300);
	uiDataTable_[ID::UP][1].pos_.x_ = uiDataTable_[ID::UP][0].pos_.x_ - uiDataTable_[ID::UP][0].imgWidth;

	uiDataTable_[ID::DOWN][0].pos_.x_ = static_cast<float>(common::screen_width) / 2;
	uiDataTable_[ID::DOWN][0].pos_.y_ = static_cast<float>((common::screen_height / 2) + 300);
	uiDataTable_[ID::DOWN][1].pos_.x_ = uiDataTable_[ID::DOWN][0].pos_.x_ + uiDataTable_[ID::DOWN][0].imgWidth;
}

Warning::~Warning()
{
	for (auto& data : uiDataTable_)
	{
		DeleteGraph(data.second[0].imgH_);
		DeleteGraph(data.second[1].imgH_);
	}
}

void Warning::Update()
{
	alphaParam_ += addAlphaValue_;
	if (!alphaParamRange_.IsInside(alphaParam_))
	{
		addAlphaValue_ *= -1;
		alphaParamRange_.Clamp(alphaParam_);
	}

	Debug::Log("alpha", alphaParam_);

	uiDataTable_[ID::UP][0].pos_.x_++;
	uiDataTable_[ID::UP][1].pos_.x_++;

	uiDataTable_[ID::DOWN][0].pos_.x_--;
	uiDataTable_[ID::DOWN][1].pos_.x_--;
}

void Warning::Draw()
{
	for (auto& data : uiDataTable_)
	{
		DrawRotaGraph(data.second[0].pos_.x_, data.second[0].pos_.y_, 1.0f, 0.0f, data.second[0].imgH_, true);
		DrawRotaGraph(data.second[1].pos_.x_, data.second[0].pos_.y_, 1.0f, 0.0f, data.second[1].imgH_, true);
	}

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, alphaParam_);
	StringManager::GetInstance().DrawStringCenter("WarningUI", common::screen_width / 2, common::screen_height / 2, 0xffffff);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}
