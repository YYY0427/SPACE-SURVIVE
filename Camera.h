#pragma once
#include <DxLib.h>

using namespace std;

class Player;

/// <summary>
/// �J�����̊Ǘ��N���X
/// </summary>
class Camera
{
public:
	// �R���X�g���N�^
	Camera(Player& pPlayer_);

	// �f�X�g���N�^
	virtual ~Camera();

	// �X�V
	void Update();

	// �`��
	void Draw();

	// �J������Y����]���̎擾
	float GetCameraYaw();

private:
	Player& pPlayer_;

	VECTOR cameraPos_;
	VECTOR cameraTarget_;

	// ��]�p�x
	float cameraYaw_;
	float cameraPitch_;
};