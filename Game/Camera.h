#pragma once
#include <DxLib.h>
#include <memory>
#include "Util/DataReaderFromUnity.h"
#include "Util/Range.h"

/// <summary>
/// �J�����̊Ǘ��N���X
/// </summary>
class Camera
{
public:
	// �R���X�g���N�^
	Camera(UnityGameObject data);

	// �f�X�g���N�^
	virtual ~Camera();

	/// <summary>
	/// �X�V
	/// </summary>
	/// <param name="playerPos">�v���C���[�̈ʒu���W</param>
	void Update(VECTOR playerPos, VECTOR playerVec);

	// �`��
	void Draw();

	// �J�����̉�]���̎擾
	float GetCameraYaw() const;// Y��

	// �J�����̈ʒu�̎擾
	VECTOR GetPos() const;

	// �J�����̒����_�̎擾
	VECTOR GetTarget() const;

private:
	// �J�����̈ʒu
	VECTOR cameraPos_;

	// �J�����̒����_
	VECTOR cameraTarget_;

	// ����p
	float perspective_;

	// ����p�͈̔�
	Range<float> perspectiveRange_;

	// ��]�p�x
	float cameraYaw_;	// Y��
	float cameraPitch_;	// X��
};