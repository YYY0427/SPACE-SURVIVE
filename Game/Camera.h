#pragma once
#include <DxLib.h>
#include <memory>
#include "Util/Range.h"

class Player;

/// <summary>
/// �J�����̊Ǘ��N���X
/// </summary>
class Camera
{
public:
	// �R���X�g���N�^
	Camera(std::shared_ptr<Player> pPlayer);

	// �f�X�g���N�^
	virtual ~Camera();

	/// <summary>
	/// �X�V
	/// </summary>
	/// <param name="playerPos">�v���C���[�̈ʒu���W</param>
	void Update();

	// �`��
	void Draw();

	// �J�����̉�]���̎擾
	float GetCameraYaw() const;// Y��

	// �J�����̈ʒu�̎擾
	VECTOR GetPos() const;

	// �J�����̒����_�̎擾
	VECTOR GetTarget() const;

private:
	std::shared_ptr<Player> pPlayer_;

	// �J�����̈ʒu
	VECTOR pos_;

	// �J�����̒����_
	VECTOR target_;

	// ����p
	float perspective_;

	// ����p�͈̔�
	Range<float> perspectiveRange_;

	// ��]�p�x
	float cameraYaw_;	// Y��
	float cameraPitch_;	// X��
};