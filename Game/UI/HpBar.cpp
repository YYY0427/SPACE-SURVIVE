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

HpBar::HpBar(float maxHp, int hpBarSideSpace, int hpBarStartY, int hpBarHeight) :
	hp_(0),
	backHp_(0),
	maxHp_(maxHp),
	aimHp_(maxHp),
	hpBarSideSpace_(hpBarSideSpace),
	hpBarStartY_(hpBarStartY),
	hpBarHeight_(hpBarHeight),
	updateFunc_(&HpBar::FirstDirectionUpdate)
{
	pos_.x = static_cast<float>(common::screen_width) / 2;
	pos_.y = hpBarStartY + (hpBarHeight / 2);

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

void HpBar::Draw()
{
	if (backHp_ > 0.0f)
	{
		DrawExtendGraph(
			hpBarSideSpace_,
			pos_.y - (hpBarHeight_ / 2),
			hpBarSideSpace_ + static_cast<int>((common::screen_width - (hpBarSideSpace_ * 2)) * (static_cast<float>(backHp_ / maxHp_))),
			pos_.y + (hpBarHeight_ / 2),
			hpBackImgH_,
			true);

		DrawExtendGraph(
			hpBarSideSpace_,
			pos_.y - (hpBarHeight_ / 2),
			hpBarSideSpace_ + static_cast<int>((common::screen_width - (hpBarSideSpace_ * 2)) * (static_cast<float>(hp_ / maxHp_))),
			pos_.y + (hpBarHeight_ / 2),
			hpImgH_,
			true);

		/*DrawExtendGraph(
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
			true);*/
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