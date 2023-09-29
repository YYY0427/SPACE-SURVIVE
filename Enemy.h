#pragma once
#include <DxLib.h>
#include <memory>

class Model;
class Player;

class Enemy
{
public:
	Enemy(int handle, VECTOR pos, VECTOR vec, float scale, std::shared_ptr<Player> pPlayer);
	virtual ~Enemy();

	void Update();
	void Draw();

	// モデルハンドルの取得
	int GetModelHandle();

	// 存在するか
	bool GetIsEnabled();
private:
	// 参照
	std::shared_ptr<Player> pPlayer_;

	// ポインタ
	std::shared_ptr<Model> pModel_;

	// 座標
	VECTOR pos_;

	// ベクトル
	VECTOR vec_;

	int timer_;

	bool isEnabled_;
};