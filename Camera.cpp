#include "Camera.h"
#include "Player.h"
#include "Util/InputState.h"

namespace
{
	// �J�����̏����ʒu
	constexpr VECTOR camera_init_pos = { 0, 150, -300 };

	// �J�����̏��������_
	constexpr VECTOR camera_init_target = { 0, 0, 0 };

	// �J�����̉�]���x
	constexpr float rot_speed = 1.0f;

	// ����p
	constexpr float perspective = 90.0f;

	// �`�拗��(near, far)
	constexpr float near_distance = 5.0f;
	constexpr float far_distance = 18400.0f;
}

/// <summary>
/// �R���X�g���N�^
/// </summary>
Camera::Camera(Player& pPlayer) :
	cameraPos_(camera_init_pos),
	cameraTarget_(camera_init_target),
	pPlayer_(pPlayer),
	cameraYaw_(0.0f), 
	cameraPitch_(0.0f)
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
	// �E�X�e�B�b�N�̓��͏��̎擾
	int up = InputState::IsXInputStick(XInputType::RIGHT, XInputTypeStick::UP);
	int down = InputState::IsXInputStick(XInputType::RIGHT, XInputTypeStick::DOWN);
	int left = InputState::IsXInputStick(XInputType::RIGHT, XInputTypeStick::LEFT);
	int right = InputState::IsXInputStick(XInputType::RIGHT, XInputTypeStick::RIGHT);

	// ���͏�񂩂�J��������]
	cameraYaw_ += (-left + right) * (rot_speed * 0.01f);
	cameraPitch_ += (-up + down) * (rot_speed * 0.01f);

	// �c��]�̉�]�p�x�̐���
	if (cameraPitch_ >= 60 * DX_PI_F / 180.0f) cameraPitch_ = 60.0f * DX_PI_F / 180.0f;
	if (cameraPitch_ <= -80 * DX_PI_F / 180.0f) cameraPitch_ = -80.0f * DX_PI_F / 180.0f;

	// ���s�s��̍쐬
	MATRIX playerTransMtx = MGetTranslate(pPlayer_.GetPos());

	// �J�����̉���]�s��̍쐬
	MATRIX cameraRotMtxSide = MGetRotY(cameraYaw_);

	// �J�����̏c��]�s��̍쐬
	MATRIX cameraRotMtxVertical = MGetRotX(cameraPitch_);

	// �J�����̉�]�s��̍쐬
	MATRIX cameraRotMtx = MMult(cameraRotMtxVertical, cameraRotMtxSide);

	// �J�����̉�]�s��ƃv���C���[�̕��s�s�񂩂�J�����̈ʒu�s��̍쐬
	MATRIX cameraMtxPos = MMult(cameraRotMtx, playerTransMtx);

	// �J�����̉�]�s��ƃv���C���[�̕��s�s�񂩂�J�����̒����_�s��̍쐬
	MATRIX cameraMtxTarget = MMult(cameraRotMtx, playerTransMtx);

	// �J�����̈ʒu�s��ƃJ�����̏����ʒu����J�����̈ʒu�̍쐬
	cameraPos_ = VTransform(camera_init_pos, cameraMtxPos);

	// �J�����̒����_�s��ƃJ�����̏��������_����J�����̒����_�̍쐬
	cameraTarget_ = VTransform(camera_init_target, cameraMtxTarget);

	// �J��������ǂꂾ�����ꂽ�Ƃ���( Near )����A �ǂ��܂�( Far )�̂��̂�`�悷�邩��ݒ�
	SetCameraNearFar(near_distance, far_distance);

	// �J�����̎���p��ݒ�(���W�A��)
	SetupCamera_Perspective(perspective * DX_PI_F / 180.0f);

	// �J�����̈ʒu�A�ǂ������Ă��邩��ݒ肷��
	SetCameraPositionAndTargetAndUpVec(cameraPos_, cameraTarget_, VGet(0, 1, 0));
}

/// <summary>
/// �`��
/// </summary>
void Camera::Draw()
{
}

/// <summary>
/// �J������Y����]���̎擾
/// </summary>
/// <returns>�J������Y����]���</returns>
float Camera::GetCameraYaw()
{
	return cameraYaw_;
}
