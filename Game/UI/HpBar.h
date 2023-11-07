#pragma once
class HpBar
{
public:
	HpBar(float maxHp);
	~HpBar();

	void Update(const float aimHpSpeed);
	void Draw(const int hpBarSideSpace, const int hpBarStartY, const int hpBarHeight);
	void OnDamage(float afterHp);

private:
	int hpFrameImgH_;
	int hpImgH_;
	int hpBackImgH_;
	float hp_;
	float backHp_;
	float aimHp_;
	const float maxHp_;
};