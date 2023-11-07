#pragma once
class EnergyGage
{
public:
	EnergyGage(float maxEnergy);
	~EnergyGage();

	void Update();
	void Draw(int x, int y, float energy);

private:
	float maxEnergy_;
	int energyBaseImgH_;
	int energyBarImgH_;
};