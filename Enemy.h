#pragma once
#include <DxLib.h>
#include <memory>

class Model;
class Player;

class Enemy
{
public:
	Enemy(VECTOR pos, VECTOR vec, float scale, Player& pPlayer);
	virtual ~Enemy();

	void Update();
	void Draw();


	int GetModelHandle();
private:
	Player& pPlayer_;
	std::shared_ptr<Model> pModel_;

	VECTOR pos_;
	VECTOR vec_;
};