#include "Camera.h"
#include "Player.h"
#include "Util/InputState.h"
#include "Util/SaveData.h"

namespace
{
	// �J�����̏����ʒu
	constexpr VECTOR camera_init_pos = { 0, 100, -250 };

	// �J�����̏��������_
	constexpr VECTOR camera_init_target = { 0, 0, 0 };

	// ����p
	constexpr float normal_perspective = 90.0f;		// �ʏ펞
	constexpr float boosting_perspective = 120.0f;	// �u�[�X�g��

	// �`�拗��(near, far)
	constexpr float near_distance = 5.0f;
	constexpr float far_distance = 30000.0f;
}

// �R���X�g���N�^
Camera::Camera(std::shared_ptr<Player> pPlayer) :
	cameraPos_(camera_init_pos),
	cameraTarget_(camera_init_target),
	pPlayer_(pPlayer),
	cameraYaw_(0.0f), 
	cameraPitch_(0.0f),
	perspective_(normal_perspective)
{
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

	// �Z�[�u�f�[�^�̊��x���̎擾
	int padStickSensX = SaveData::GetInstance().GetSaveData().padStickSensitivityX;
	int padStickSensY = SaveData::GetInstance().GetSaveData().padStickSensitivityY;

	// �Z�[�u�f�[�^�̃��o�[�X���̎擾
	bool padStickReversX = SaveData::GetInstance().GetSaveData().padStickReverseX;
	bool padStickReversY = SaveData::GetInstance().GetSaveData().padStickReverseY;

	int x = 1, y = 1;
	if (padStickReversX)	x *= -1;
	if (padStickReversY)	y *= -1;

	// ���͏�񂩂�J��������]
	cameraYaw_ += (-left + right) * (padStickSensX * 0.002f) * x;
	cameraPitch_ += (up + -down) * (padStickSensY * 0.002f) * y;
	
	// �c��]�̉�]�p�x�̐���
	if (cameraPitch_ >= 60 * DX_PI_F / 180.0f)
	{
		// ������̉�]�p�x�̐���
		cameraPitch_ = 60.0f * DX_PI_F / 180.0f;
	}
	if (cameraPitch_ <= -80 * DX_PI_F / 180.0f)
	{
		// �������̉�]�p�x�̐���
		cameraPitch_ = -80.0f * DX_PI_F / 180.0f;
	}

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

	// �J�����̈ʒu�s��ƃJ�����̏����ʒu����J�����̈ʒu�̍쐬
	cameraPos_ = VTransform(camera_init_pos, cameraMtxPos);

	// �J�����̒����_�s��ƃJ�����̏��������_����J�����̒����_�̍쐬
	cameraTarget_ = VTransform(camera_init_target, cameraMtxTarget);

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
float Camera::GetCameraYaw()
{
	return cameraYaw_;
}

float Camera::GetCameraFar() const
{
	return far_distance;
}

// �J�����̈ʒu�̎擾
VECTOR Camera::GetPos()
{
	return cameraPos_;
}

// �J�����̒����_�̎擾
VECTOR Camera::GetTarget()
{
	return cameraTarget_;
}
