#pragma once
#include "../Util/Timer.h"
#include "../UIBase.h"

class HpBar : public UIBase
{
public:
	HpBar(float maxHp, int hpBarSideSpace, int hpBarStartY, int hpBarHeight);
	~HpBar();

	void Update(const float aimHpSpeed);
	void Draw();
	void OnDamage(float afterHp);

	/// <summary>
	/// HP�������Ă������o���I�������
	/// </summary>
	/// <returns> true : �I���Afalse : �r�� </returns>
	bool IsEndFirstDirection() const;

private:
	void NormalUpdate(const float aimHpSpeed);
	void FirstDirectionUpdate(const float aimHpSpeed);

private:
	void(HpBar::*updateFunc_)(const float aimHpSpeed);

	int hpFrameImgH_;
	int hpImgH_;
	int hpBackImgH_;

	float hp_;
	float backHp_;
	float aimHp_;
	const float maxHp_;

	int hpBarSideSpace_;
	int hpBarStartY_;
	int hpBarHeight_;

	Timer<int> damageTimer_;
};