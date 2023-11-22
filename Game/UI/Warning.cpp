#include "Warning.h"
#include "../Util/DrawFunctions.h"
#include "../Util/StringManager.h"
#include "../common.h"
#include "../Util/Debug.h"
#include <string>
#include <DxLib.h>

namespace
{
	// �摜�̃t�@�C���p�X
	const std::string center_img_file_path = "Data/Image/Warning.png";
	const std::string up_img_file_path = "Data/Image/WarningUp.png";
	const std::string down_img_file_path = "Data/Image/WarningDown.png";

	// �����̃t�F�[�h�̑��x
	constexpr int string_fade_speed = 5;

	// �㉺�̃o�[�̃X�N���[�����x
	constexpr int scroll_speed = 10;

	// �S�̂̃t�F�[�h���x
	constexpr int fade_speed = 5;
}

Warning::Warning(int drawFrame) :
	stringAlphaParam_(255),
	addStringAlphaValue_(string_fade_speed),
	stringAlphaParamRange_(10, 255),
	scroll_(0),
	imgAlphaParam_(0),
	addImgAlphaValue_(fade_speed),
	imgAlphaParamRange_(0, 255),
	drawFrameTimer_(drawFrame),
	isEnd_(false)
{
	// �摜�̃��[�h
	uiDataTable_[static_cast<int>(ID::CENTER)].imgH = my::MyLoadGraph(center_img_file_path.c_str());
	uiDataTable_[static_cast<int>(ID::UP)].imgH = my::MyLoadGraph(up_img_file_path.c_str());
	uiDataTable_[static_cast<int>(ID::DOWN)].imgH = my::MyLoadGraph(down_img_file_path.c_str());

	// �O���t�B�b�N�̃T�C�Y�̎擾
	for (auto& data : uiDataTable_)
	{
		GetGraphSize(data.imgH, &data.imgWidth, &data.imgHeight);
	}

	// ���W�̏�����
	uiDataTable_[static_cast<int>(ID::CENTER)].pos.x_ = static_cast<float>(common::screen_width) / 2;
	uiDataTable_[static_cast<int>(ID::CENTER)].pos.y_ = static_cast<float>(common::screen_height) / 2;
	uiDataTable_[static_cast<int>(ID::UP)].pos.y_ = static_cast<float>((common::screen_height / 2) - 300);
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

bool Warning::IsEnd() const
{
	return (isEnd_) && (imgAlphaParam_ <= 0);
}

void Warning::Update()
{
	// �`�掞�Ԃ̃J�E���g
	drawFrameTimer_.Update(1);
	if (drawFrameTimer_.IsTimeOut() && !isEnd_)
	{
		isEnd_ = true;
		addImgAlphaValue_ *= -1;
		addStringAlphaValue_ = -fade_speed;
	}
	else if(!drawFrameTimer_.IsTimeOut())
	{
		if (!stringAlphaParamRange_.IsInside(stringAlphaParam_))
		{
			addStringAlphaValue_ *= -1;
			stringAlphaParam_ = stringAlphaParamRange_.Clamp(stringAlphaParam_);
		}
	}

	// �����̃t�F�[�h
	stringAlphaParam_ += addStringAlphaValue_;

	// �㉺�̃o�[�̃X�N���[��
	scroll_ += scroll_speed;

	// �摜�̃t�F�[�h
	imgAlphaParam_ += addImgAlphaValue_;
	imgAlphaParam_ = imgAlphaParamRange_.Clamp(imgAlphaParam_);
}

void Warning::Draw()
{
	for (auto& data : uiDataTable_)
	{
		int	scroll = static_cast<int>(scroll_ * data.scrollDir) % static_cast<int>(data.imgWidth);

		if (data.scrollDir != 0)
		{
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, imgAlphaParam_);
			DrawRotaGraph(
				(data.imgWidth * 0.5) + scroll,
				data.pos.y_,
				1.0f, 0.0f, data.imgH, true);

			DrawRotaGraph(
				(data.imgWidth * (1 * -data.scrollDir + 0.5)) + scroll,
				data.pos.y_,
				1.0f, 0.0f, data.imgH, true);
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		}
		else
		{
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, imgAlphaParam_);
			DrawRotaGraph(
				data.pos.x_,
				data.pos.y_,
				1.0f, 0.0f, data.imgH, true);
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		}
	}

	float ratio;
	(!isEnd_) ?
		(ratio = (static_cast<float>(stringAlphaParam_) / static_cast<float>(imgAlphaParam_) * 255.0f)) :
		(ratio = stringAlphaParam_);
	Debug::Log("ratio", stringAlphaParam_);

	SetDrawBlendMode(DX_BLENDMODE_ALPHA,  ratio);
	StringManager::GetInstance().DrawStringCenter("WarningUI", common::screen_width / 2, common::screen_height / 2, 0xffffff);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}
