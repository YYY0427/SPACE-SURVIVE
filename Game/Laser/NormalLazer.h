#pragma once
#include "LazerBase.h"

class NormalLazer : public LazerBase
{
public:
	NormalLazer(int modelHandle);
	virtual ~NormalLazer();

	void Update() override;
	void Draw() override;

	void Fire(const VECTOR pos, const VECTOR vec, const VECTOR rot) override;
	void Refrect() override;

private:
	int lazerEffectHandle_;
};

