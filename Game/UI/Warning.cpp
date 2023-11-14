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
	alphaParamRange_(10, 255),
	scroll_(0)
{
	// 画像のロード
	uiDataTable_[static_cast<int>(ID::CENTER)].imgH = my::MyLoadGraph(center_img_file_path.c_str());
	uiDataTable_[static_cast<int>(ID::UP)].imgH = my::MyLoadGraph(up_img_file_path.c_str());
	uiDataTable_[static_cast<int>(ID::DOWN)].imgH = my::MyLoadGraph(down_img_file_path.c_str());

	// グラフィックのサイズの取得
	for (auto& data : uiDataTable_)
	{
		GetGraphSize(data.imgH, &data.imgWidth, &data.imgHeight);
	}

	// 座標の初期化
	uiDataTable_[static_cast<int>(ID::CENTER)].pos.x_ = static_cast<float>(common::screen_width) / 2;
	uiDataTable_[static_cast<int>(ID::CENTER)].pos.y_ = static_cast<float>(common::screen_height) / 2;

	uiDataTable_[static_cast<int>(ID::UP)].pos.x_ = static_cast<float>(common::screen_width) / 2;
	uiDataTable_[static_cast<int>(ID::UP)].pos.y_ = static_cast<float>((common::screen_height / 2) - 300);

	uiDataTable_[static_cast<int>(ID::DOWN)].pos.x_ = static_cast<float>(common::screen_width) / 2;
	uiDataTable_[static_cast<int>(ID::DOWN)].pos.y_ = static_cast<float>((common::screen_height / 2) + 300);

	uiDataTable_[static_cast<int>(ID::CENTER)].scrollDir = 0;
	uiDataTable_[static_cast<int>(ID::UP)].scrollDir = 1;
	uiDataTable_[static_cast<int>(ID::DOWN)].scrollDir = -1;
}

Warning::~Warning()
{
	for (auto& data : uiDataTable_)
	{
		DeleteGraph(data.imgH);
	}
}

void Warning::Update()
{
	alphaParam_ += addAlphaValue_;
	Debug::Log("alpha", alphaParam_);
	if (!alphaParamRange_.IsInside(alphaParam_))
	{
		addAlphaValue_ *= -1;
		alphaParamRange_.Clamp(alphaParam_);
	}

	scroll_ += 10;
}

void Warning::Draw()
{
	for (auto& data : uiDataTable_)
	{
		int scroll = static_cast<int>(scroll_ * data.scrollDir) % static_cast<int>(data.imgWidth);

		DrawRotaGraph(
			(data.imgWidth * 0.5) + scroll,
			data.pos.y_,
			1.0f, 0.0f, data.imgH, true);

		DrawRotaGraph(
			(data.imgWidth * (1.5)) + scroll,
			data.pos.y_,
			1.0f, 0.0f, data.imgH, true);
	}

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, alphaParam_);
	StringManager::GetInstance().DrawStringCenter("WarningUI", common::screen_width / 2, common::screen_height / 2, 0xffffff);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}
