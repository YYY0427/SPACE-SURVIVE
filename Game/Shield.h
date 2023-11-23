#pragma once
#include <memory>
#include <DxLib.h>
#include <array>

class Player;
class Image3D;
class EnergyGage;

class Shield
{
public:
	Shield(Player& player);
	~Shield();

	void Update();
	void Draw();
	void DrawUI();

	VECTOR GetPos() const;

	bool GetIsShield() const;

	std::array<VERTEX3D, 6> GetVertex() const;

private:
	std::shared_ptr<Image3D> pShiled_;
	std::unique_ptr<EnergyGage> pEnergyGage_;
	Player& player_;

	int effectHandle_;
	int enerugyGage_;

	VECTOR pos_;
	VECTOR rot_;

	// �v���C���[����̑��Έʒu
	VECTOR relativePosToPlayer_;

	// ���͂��ꂽ��
	bool isInput_;
};