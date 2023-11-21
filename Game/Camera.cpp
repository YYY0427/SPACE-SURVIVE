#include "Camera.h"
#include "Player.h"
#include "Util/InputState.h"
#include "Util/SaveData.h"

namespace
{
	// �J�����̏����ʒu
	constexpr VECTOR camera_pos = { 0, 0, -300 };

	// �J�����̏��������_
	constexpr VECTOR camera_init_target = { 0, 0, 0 };

	// ����p
	constexpr float normal_perspective = 90.0f;		// �ʏ펞
	constexpr float boosting_perspective = 120.0f;	// �u�[�X�g��

	// �`�拗��(near, far)
	constexpr float near_distance = 5.0f;
	constexpr float far_distance = 20000.0f;
}

// �R���X�g���N�^
Camera::Camera() :
	cameraYaw_(0.0f),
	cameraPitch_(0.0f),
	perspective_(normal_perspective),
	perspectiveRange_({ normal_perspective, boosting_perspective })
{
	pos_ = camera_pos;
	target_ = camera_init_target;
}

// �f�X�g���N�^
Camera::~Camera()
{
}

// �X�V
void Camera::Update()
{
	// �J��������ǂꂾ�����ꂽ�Ƃ���( Near )����A �ǂ��܂�( Far )�̂��̂�`�悷�邩��ݒ�
	SetCameraNearFar(near_distance, far_distance);

	// �J�����̎���p��ݒ�(���W�A��)
	SetupCamera_Perspective(perspective_ * DX_PI_F / 180.0f);

	// �J�����̈ʒu�A�ǂ������Ă��邩��ݒ肷��
	SetCameraPositionAndTargetAndUpVec(pos_, target_, VGet(0, 1, 0));
}

// �`��
void Camera::Draw()
{
}

// �J������Y����]���̎擾
float Camera::GetCameraYaw() const 
{
	return cameraYaw_;
}

// �J�����̈ʒu�̎擾
VECTOR Camera::GetPos() const 
{
	return pos_;
}

// �J�����̒����_�̎擾
VECTOR Camera::GetTarget() const 
{
	return target_;
}
