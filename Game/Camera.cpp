#include "Camera.h"
#include "Player.h"
#include "Util/InputState.h"
#include "Util/SaveData.h"

namespace
{
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
Camera::Camera(std::shared_ptr<Player> pPlayer, UnityGameObject data) :
	cameraPos_(data.pos),
	cameraTarget_(camera_init_target),
	cameraYaw_(0.0f),
	cameraPitch_(0.0f),
	perspective_(normal_perspective),
	perspectiveRange_({ normal_perspective, boosting_perspective }),
	pPlayer_(pPlayer)
{
	// ���s�s��̍쐬
	MATRIX playerTransMtx = MGetTranslate(VScale(pPlayer_->GetPos(), 1.0f));

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

	// �J�����̒����_�s��ƃJ�����̏��������_����J�����̒����_�̍쐬
	cameraTarget_ = VTransform(camera_init_target, cameraMtxTarget);
}

// �f�X�g���N�^
Camera::~Camera()
{
}

// �X�V
void Camera::Update()
{
	// �E�X�e�B�b�N�̓��͏��̎擾
	int up = InputState::IsPadStick(PadLR::RIGHT, PadStickInputType::UP);
	int down = InputState::IsPadStick(PadLR::RIGHT, PadStickInputType::DOWN);
	int left = InputState::IsPadStick(PadLR::RIGHT, PadStickInputType::LEFT);
	int right = InputState::IsPadStick(PadLR::RIGHT, PadStickInputType::RIGHT);

	if (InputState::IsPadStick(PadLR::LEFT, PadStickInputType::UP))
	{
//		perspective_ += 1.5f;
	}
	else
	{
//		perspective_ -= 1.5f;
	}
	perspective_ = perspectiveRange_.Clamp(perspective_);

	VECTOR playerPos = pPlayer_->GetPos();
	VECTOR playerVec = pPlayer_->GetMoveVec();

	VECTOR toPlayerVec = VSub(playerPos, cameraPos_);
	toPlayerVec.y = 0.0f;

	if (VSize(toPlayerVec) > 1000.0f && 
		InputState::IsPadStick(PadLR::LEFT, PadStickInputType::UP))
	{
		playerVec.x = 0.0f;
		cameraPos_ = VAdd(cameraPos_, playerVec);
		cameraTarget_ = VAdd(cameraTarget_, playerVec);
	}

	// �J��������ǂꂾ�����ꂽ�Ƃ���( Near )����A �ǂ��܂�( Far )�̂��̂�`�悷�邩��ݒ�
	SetCameraNearFar(near_distance, far_distance);

	// �J�����̎���p��ݒ�(���W�A��)
	SetupCamera_Perspective(perspective_ * DX_PI_F / 180.0f);

	// �J�����̈ʒu�A�ǂ������Ă��邩��ݒ肷��
	SetCameraPositionAndTargetAndUpVec(cameraPos_, cameraTarget_, VGet(0, 1, 0));
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
	return cameraPos_;
}

// �J�����̒����_�̎擾
VECTOR Camera::GetTarget() const 
{
	return cameraTarget_;
}
