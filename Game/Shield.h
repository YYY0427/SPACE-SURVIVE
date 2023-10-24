#pragma once
#include <memory>
#include <DxLib.h>

class Player;
class Model;

class Shield
{
//	friend Player;
public:
	Shield(Player& player);
	virtual ~Shield();

	void Update();
	void Draw();

private:
	std::shared_ptr<Model> pModel_;
	Player& player_;

	int effectHandle_;

	VECTOR pos_;
	VECTOR rot_;

	// �v���C���[����̑��Έʒu
	VECTOR relativePosToPlayer_;

	// �V�[���h���o���Ă��邩
	bool isShield_;
};