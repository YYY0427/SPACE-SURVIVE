#pragma once
#include <DxLib.h>
#include <memory>
#include "Util/Range.h"
#include "Util/Timer.h"

/// <summary>
/// �J�����̊Ǘ��N���X
/// </summary>
class Camera
{
public:
	// �R���X�g���N�^
	Camera();

	// �f�X�g���N�^
	~Camera();

	/// <summary>
	/// �X�V
	/// </summary>
	/// <param name="playerPos">�v���C���[�̈ʒu���W</param>
	void Update();

	// �Q�[���N���A���̍X�V
	void GameClearUpdate(VECTOR playerPos);

	// �J���������߂�ꂽ�t���[���̊ԁAx���ɗh�炷
	void ShakeX(int shakeFrame, float shakeSize);

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
	VECTOR pos_;

	// �J�����̒����_
	VECTOR target_;

	// ����p
	float perspective_;

	// ����p�͈̔�
	Range<float> perspectiveRange_;

	// �J������h�炷�t���[����
	Timer<int> shakeFrame_;

	float cameraVertical_;	// ��������
	float cameraHorizon_;	// ��������

	// ��]�p�x
	float cameraYaw_;	// Y��
	float cameraPitch_;	// X��
};