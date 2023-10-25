#pragma once
#include <DxLib.h>
#include <memory>

/// <summary>
/// �J�����̊Ǘ��N���X
/// </summary>
class Camera
{
public:
	// �R���X�g���N�^
	Camera();

	// �f�X�g���N�^
	virtual ~Camera();

	/// <summary>
	/// �X�V
	/// </summary>
	/// <param name="playerPos">�v���C���[�̈ʒu���W</param>
	void Update(VECTOR playerPos);

	// �`��
	void Draw();

	// �J�����̉�]���̎擾
	float GetCameraYaw();	// Y��

	float GetCameraFar() const;

	// �J�����̈ʒu�̎擾
	VECTOR GetPos();

	// �J�����̒����_�̎擾
	VECTOR GetTarget();

private:
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