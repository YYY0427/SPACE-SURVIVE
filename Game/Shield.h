#pragma once
#include <memory>
#include <DxLib.h>
#include <array>

class Player;
class Image3D;

class Shield
{
public:
	Shield(Player& player);
	virtual ~Shield();

	void Update();
	void Draw();

	VECTOR GetPos() const;

	bool GetIsShield() const;

	std::array<VERTEX3D, 6> GetVertex() const;

private:
	std::shared_ptr<Image3D> pShiled_;
	Player& player_;

	int effectHandle_;
	int enerugyGage_;

	VECTOR pos_;
	VECTOR rot_;

	// プレイヤーからの相対位置
	VECTOR relativePosToPlayer_;

	// 入力されたか
	bool isInput_;
};