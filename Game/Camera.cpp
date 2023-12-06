#include "Camera.h"
#include "Player.h"
#include "Util/InputState.h"
#include "Util/SaveData.h"
#include "Util/MathUtil.h"

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

	// �v���C���[����̋���
	constexpr float camera_distance = 300.0f;

	// �Q�[���N���A���̃J�����̉�]���x
	constexpr float camera_rotate_speed = 0.03f;
}

// �R���X�g���N�^
Camera::Camera() :
	cameraYaw_(0.0f),
	cameraPitch_(0.0f),
	perspective_(normal_perspective),
	perspectiveRange_({ normal_perspective, boosting_perspective }),
	cameraVertical_(0.0f),
	cameraHorizon_(DX_PI_F)
{
	pos_ = camera_pos;
	target_ = camera_init_target;

//	stateMachine_.AddState(State::Normal, std::bind(&Camera::UpdateNormalState, this), std::bind(&Camera::EntarNormalState, this), std::bind(&Camera::ExitNormalState, this));
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

// �Q�[���N���A���̍X�V
void Camera::GameClearUpdate(VECTOR playerPos)
{
	cameraHorizon_ -= camera_rotate_speed;
	cameraHorizon_ = (std::max)(cameraHorizon_, MathUtil::DegreeFromRadian(30));

	// �J�����̒����_���v���C���[�̈ʒu�ɐݒ�
	goalTarget_ = playerPos;

	// ��̒����𐂒������ɉ�]�������Ƃ��̐������̒���
	float verticalLength = camera_distance * cosf(cameraVertical_);

	// ����
	float horizonLength = camera_distance * sinf(cameraVertical_);

	// �J�������W�̐ݒ�
	// xz���W�͐��������̒������i�߂��Ƃ���
	goalPos_.x = playerPos.x + verticalLength * sinf(cameraHorizon_);
	goalPos_.z = playerPos.z + verticalLength * cosf(cameraHorizon_);

	// �x���W�͐������������
	goalPos_.y = playerPos.y + horizonLength;

	VECTOR targetVec = VScale(VNorm(VSub(goalTarget_, target_)), 3.0f);
	target_ = VAdd(target_, targetVec);

	VECTOR moveVec = VScale(VNorm(VSub(goalPos_, pos_)), 3.0f);
	pos_ = VAdd(pos_, moveVec);

	// �J��������ǂꂾ�����ꂽ�Ƃ���( Near )����A �ǂ��܂�( Far )�̂��̂�`�悷�邩��ݒ�
	SetCameraNearFar(near_distance, far_distance);

	// �J�����̎���p��ݒ�(���W�A��)
	SetupCamera_Perspective(MathUtil::DegreeFromRadian(perspective_));

	// �J�����̈ʒu�A�ǂ������Ă��邩��ݒ肷��
	SetCameraPositionAndTargetAndUpVec(pos_, target_, VGet(0, 1, 0));
}

void Camera::ShakeX(int shakeFrame, float shakeSize)
{
	shakeFrame_ = shakeFrame;
	shakeFrame_.Update(1);
	if (!shakeFrame_.IsTimeOut())
	{
		pos_.x += sinf(GetNowCount() / 10.0f) * shakeSize;
		shakeSize *= 0.96f;
	}
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

void Camera::EntarNormalState()
{
}

void Camera::EntarGameClearState()
{
}

void Camera::UpdateNormalState()
{
}

void Camera::UpdateGameClearState()
{
}

void Camera::ExitNormalState()
{
}

void Camera::ExitGameClearState()
{
}
