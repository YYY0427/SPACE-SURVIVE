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

	// ���f���n���h���̎擾
	int GetModelHandle();

	// ���݂��邩
	bool GetIsEnabled();
private:
	// �Q��
	Player& pPlayer_;

	// �|�C���^
	std::shared_ptr<Model> pModel_;

	// ���W
	VECTOR pos_;

	// �x�N�g��
	VECTOR vec_;

	int timer_;

	bool isEnabled_;
};