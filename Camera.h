#pragma once
#include <DxLib.h>
#include <memory>

class Player;

/// <summary>
/// �J�����̊Ǘ��N���X
/// </summary>
class Camera
{
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="pPlayer">�v���C���[�̃|�C���^</param>
	Camera(std::shared_ptr<Player> pPlayer);

	// �f�X�g���N�^
	virtual ~Camera();

	// �X�V
	void Update();

	// �`��
	void Draw();

	// �J�����̉�]���̎擾
	float GetCameraYaw();	// Y��

	// �J�����̈ʒu�̎擾
	VECTOR GetPos();

	// �J�����̒����_�̎擾
	VECTOR GetTarget();
private:
	// �|�C���^
	std::shared_ptr<Player> pPlayer_;

	// �J�����̈ʒu
	VECTOR cameraPos_;

	// �J�����̒����_
	VECTOR cameraTarget_;

	// ����p
	float perspective_;

	// ��]�p�x
	float cameraYaw_;	// Y��
	float cameraPitch_;	// X��
};