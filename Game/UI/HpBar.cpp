#include "HpBar.h"
#include "../common.h"
#include "../Util/DrawFunctions.h"
#include "../Util/Debug.h"
#include <DxLib.h>
#include <string>

// TODO : ダメージを受けている間はbackHPは減らないようにする
// ダメージを受けてから決められたフレームの間ダメージを受けなかったらbackHPを減らすように変更して

namespace
{
	const std::string hp_frame_img_file_path = "Data/Image/HPFrame.png";
	const std::string hp1_img_file_path = "Data/Image/HP.png";
	const std::string hp2_img_file_path = "Data/Image/HPBack.png";

	constexpr float add_speed = 50.0f;
}

HpBar::HpBar(float maxHp) :
	maxHp_(maxHp),
	backHp_(0),
	aimHp_(maxHp),
	hp_(0),
	updateFunc_(&HpBar::FirstDirectionUpdate)
{
	hpFrameImgH_ = my::MyLoadGraph(hp_frame_img_file_path.c_str());
	hpImgH_ = my::MyLoadGraph(hp1_img_file_path.c_str());
	hpBackImgH_ = my::MyLoadGraph(hp2_img_file_path.c_str());
}

HpBar::~HpBar()
{
}

void HpBar::Update(const float aimHpSpeed)
{
	(this->*updateFunc_)(aimHpSpeed);
}

void HpBar::Draw(const int hpBarSideSpace, const int hpBarStartY, const int hpBarHeight)
{
	if (backHp_ > 0.0f)
	{
		DrawExtendGraph(
			hpBarSideSpace,
			hpBarStartY,
			hpBarSideSpace + static_cast<int>((common::screen_width - (hpBarSideSpace * 2)) * (static_cast<float>(backHp_ / maxHp_))),
			hpBarStartY + hpBarHeight,
			hpBackImgH_,
			true);

		DrawExtendGraph(
			hpBarSideSpace,
			hpBarStartY,
			hpBarSideSpace + static_cast<int>((common::screen_width - (hpBarSideSpace * 2)) * (static_cast<float>(hp_ / maxHp_))),
			hpBarStartY + hpBarHeight,
			hpImgH_,
			true);
	}
	/*DrawExtendGraph(
		hpBarSideSpace,
		hpBarStartY,
		hpBarSideSpace + static_cast<int>((common::screen_width - (hpBarSideSpace * 2))),
		hpBarStartY + hpBarHeight,
		hpFrameImgH_,
		true);*/
}

void HpBar::OnDamage(float afterHp)
{
	aimHp_ = afterHp;
	damageTimer_.SetTime(60);
}

bool HpBar::IsEndFirstDirection() const
{
	return (updateFunc_ != &HpBar::FirstDirectionUpdate);
}

void HpBar::NormalUpdate(const float aimHpSpeed)
{
	hp_ = aimHp_;
	hp_ = (std::max)(hp_, 0.0f);

	damageTimer_.Update(-1);
	if (damageTimer_.GetTime() <= 0)
	{
		if (backHp_ != aimHp_)
		{
			// 毎フレーム緩やかに目標に近づく
			backHp_ -= aimHpSpeed;

			// 目標に合致したら止める
			if (backHp_ < aimHp_)
			{
				backHp_ = aimHp_;
			}
		}
	}
	Debug::Log("aimHp", aimHp_);
}

void HpBar::FirstDirectionUpdate(const float aimHpSpeed)
{
	float speed = add_speed / maxHp_;
	hp_ += speed;
	backHp_ += speed;

	if (hp_ >= maxHp_ && backHp_ >= maxHp_)
	{
		hp_ = maxHp_;
		backHp_ = maxHp_;

		updateFunc_ = &HpBar::NormalUpdate;
	}
}