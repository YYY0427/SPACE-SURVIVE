#pragma once

// シールドのエネルギーゲージクラス
class EnergyGage
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="maxEnergy">エネルギー最大容量</param>
	EnergyGage(float maxEnergy);

	// デストラクタ
	~EnergyGage();

	/// <summary>
	/// エネルギーゲージの描画
	/// </summary>
	/// <param name="x">描画したいx座標</param>
	/// <param name="y">描画したいy座標</param>
	/// <param name="energy">現在のエネルギー残量</param>
	void Draw(int x, int y, float energy);

private:
	// エネルギーの最大容量
	float maxEnergy_;

	// エネルギーゲージの画像ハンドル
	int energyBaseImgH_;
	int energyBarImgH_;
};