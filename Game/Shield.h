#pragma once
#include <memory>
#include <DxLib.h>
#include "Util/Timer.h"

class Player;
class Model;

class Shield
{
public:
	Shield(Player& player);
	virtual ~Shield();

	void Update();
	void Draw();

	VECTOR GetPos() const;

	float GetCollisonRadius() const;

	bool GetIsShield() const;

private:
	std::shared_ptr<Model> pModel_;
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