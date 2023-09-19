#pragma once
#include <DxLib.h>

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

	// �J�����̉�]���̎擾
	float GetCameraYaw();	// Y��
	float GetCameraPitch();	// X��

	VECTOR GetPos();
	VECTOR GetTarget();
private:
	Player& pPlayer_;

	VECTOR cameraPos_;
	VECTOR cameraTarget_;

	// ����p
	float perspective_;

	// ��]�p�x
	float cameraYaw_;
	float cameraPitch_;
};