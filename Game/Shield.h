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

	// プレイヤーからの相対位置
	VECTOR relativePosToPlayer_;

	// シールドを出しているか
	bool isShield_;
};