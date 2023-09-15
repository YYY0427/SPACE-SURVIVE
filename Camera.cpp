#include "Camera.h"
#include <DxLib.h>

namespace
{
	// ����p
	constexpr float perspective = 60.0f;

	// �`�拗��(near, far)
	constexpr float near_distance = 5.0f;
	constexpr float far_distance = 18400.0f;
}

/// <summary>
/// �R���X�g���N�^
/// </summary>
Camera::Camera()
{
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
Camera::~Camera()
{
}

/// <summary>
/// �X�V
/// </summary>
void Camera::Update()
{


	// �J��������ǂꂾ�����ꂽ�Ƃ���( Near )����A �ǂ��܂�( Far )�̂��̂�`�悷�邩��ݒ�
	SetCameraNearFar(near_distance, far_distance);

	// �J�����̎���p��ݒ�(���W�A��)
	SetupCamera_Perspective(perspective * DX_PI_F / 180.0f);

	// �J�����̈ʒu�A�ǂ������Ă��邩��ݒ肷��
//	SetCameraPositionAndTarget_UpVecY(cameraPos, cameraTarget);
}

/// <summary>
/// �`��
/// </summary>
void Camera::Draw()
{
}