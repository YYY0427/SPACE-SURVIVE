#pragma once
#include <DxLib.h>
#include <memory>

using namespace std;

class Model;

/// <summary>
/// �v���C���[�N���X
/// </summary>
class Player
{
public:
	// �R���X�g���N�^
	Player();

	// �f�X�g���N�^
	virtual ~Player();

	// �X�V
	void Update();

	// �`��
	void Draw();
private:
	shared_ptr<Model> pModel_;

	// �ʒu���
	VECTOR pos_;
};