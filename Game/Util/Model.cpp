#include "Model.h"
#include <cassert>

namespace
{
	// �����蔻��Ƃ��Ďg�p����t���[���̖��O
	const std::string collision_frame_name = "Collision";
}

// �t�@�C�������w�肵�ă��[�h���s��
Model::Model(std::string fileName) :
	isUseCollision_(false),
	isUpdateColision_(false),
	colFrameIndex_(-1),
	animChangeFrame_(0),
	animChangeFrameTotal_(0)
{
	// ���f���̃��[�h
	modelHandle_ = MV1LoadModel(fileName.c_str());

	// ���f���̃��[�h�Ɏ��s����������~�߂�
	assert(modelHandle_ != -1);

	// �A�j���[�V�������̏�����
	InitAnimData(animPrev_);
	InitAnimData(animNext_);
}

// �w�肳�ꂽ�n���h���̃��f�����R�s�[���Đ�������
Model::Model(int modelHandle) :
	isUseCollision_(false),
	isUpdateColision_(false),
	colFrameIndex_(-1),
	animChangeFrame_(0),
	animChangeFrameTotal_(0)
{
	// ���f���̃R�s�[
	modelHandle_ = MV1DuplicateModel(modelHandle);

	// ���f���̃R�s�[�Ɏ��s������~�߂�
	assert(modelHandle_ != -1);

	// �A�j���[�V�������̏�����
	InitAnimData(animPrev_);
	InitAnimData(animNext_);
}

// �f�X�g���N�^
Model::~Model()
{
	// �����蔻������g�p���Ă�����폜
	if (isUpdateColision_)
	{
		MV1TerminateCollInfo(modelHandle_, -1);
		isUseCollision_ = false;
	}

	// ���f���̍폜
	MV1DeleteModel(modelHandle_);
}

// �����蔻��̐ݒ�
void Model::SetUseCollision(bool isUse, bool isNeedUpdate)
{
	// �����蔻����g��Ȃ��̂ɓ����蔻��̍X�V�𖈃t���[�������Ȃ������ꍇ���͂���
	assert(isUse || !isNeedUpdate);

	// �����蔻��g�p���ɕω����������ꍇ�̂ݍX�V����
	if (isUpdateColision_ != isUse)
	{
		if (isUse)
		{
			// �g��Ȃ����g��
			// �����蔻��p�t���[��������
			colFrameIndex_ = MV1SearchFrame(modelHandle_, collision_frame_name.c_str());

			// ������Ȃ����� or �G���[
			if (colFrameIndex_ < 0)	
			{
				colFrameIndex_ = -1;
			}

			// �����蔻������\�z����
			MV1SetupCollInfo(modelHandle_, colFrameIndex_, 8, 8, 8);
		}
		else
		{
			// �g�����g��Ȃ�
			MV1TerminateCollInfo(modelHandle_, -1);
		}
	}

	// �ۑ�
	isUseCollision_ = isUse;
	isUpdateColision_ = isNeedUpdate;
}

// �X�V
void Model::Update()
{
	UpdateAnim(animPrev_);
	UpdateAnim(animNext_);

	// �w��t���[�������ăA�j���[�V������ύX����
	animChangeFrame_++;
	if (animChangeFrame_ > animChangeFrameTotal_)
	{
		animChangeFrame_ = animChangeFrameTotal_;
	}

	// �A�j���[�V�����̃u�����h����ݒ肷��
	UpdateAnimBlendRate();

	// �����蔻��f�[�^�̍X�V
	if (isUseCollision_ && isUpdateColision_)
	{
		MV1RefreshCollInfo(modelHandle_, colFrameIndex_);
	}
}

//  �`��
void Model::Draw()
{
	MV1DrawModel(modelHandle_);
}

// ���f���̕\���ʒu�̐ݒ�
void Model::SetPos(VECTOR pos)
{
	MV1SetPosition(modelHandle_, pos);
}

// ���f���̉�]��Ԃ̐ݒ�
void Model::SetRot(VECTOR rot)
{
	MV1SetRotationXYZ(modelHandle_, rot);
}

// ���f���̊g�嗦�̐ݒ�
void Model::SetScale(VECTOR scale)
{
	MV1SetScale(modelHandle_, scale);
}

// ���f���̕s�����x�̐ݒ�
void Model::SetOpacity(float opacity)
{
	MV1SetOpacityRate(modelHandle_, opacity);
}

// �A�j���[�V������ݒ肷��(�ς��Ɛ؂�ւ���)
void Model::SetAnimation(int animNo, bool isLoop, bool isForceChange, float animPlaySpeed)
{
	if (!isForceChange)
	{
		// ���łɍĐ�����Ă���A�j���͍Đ����Ȃ�
		if (animNext_.animNo == animNo)	return;
	}

	// �ȑO�̃A�j���[�V�������c���Ă���ΏI��
	if (animPrev_.attachNo != -1)
	{
		MV1DetachAnim(modelHandle_, animPrev_.attachNo);
		InitAnimData(animPrev_);
	}
	if (animNext_.attachNo != -1)
	{
		MV1DetachAnim(modelHandle_, animNext_.attachNo);
		InitAnimData(animNext_);
	}

	// �V�����A�j���[�V������ݒ�
	animNext_.animNo = animNo;
	animNext_.playSpeed = animPlaySpeed;
	animNext_.attachNo = MV1AttachAnim(modelHandle_, animNo, -1, false);
	animNext_.totalTime = MV1GetAttachAnimTotalTime(modelHandle_, animNext_.attachNo);
	animNext_.isLoop = isLoop;

	animChangeFrameTotal_ = 1;
	animChangeFrame_ = 1;
}

// �A�j���[�V������ω�������(���t���[�������Đ؂�ւ���)
void Model::ChangeAnimation(int animNo, bool isLoop, bool isForceChange, int changeFrame, float animPlaySpeed)
{
	if (!isForceChange)
	{
		// ���łɍĐ�����Ă���A�j���͍Đ����Ȃ�
		if (animNext_.animNo == animNo)	return;
	}

	// �ȑO�̃A�j���[�V�������c���Ă���ΏI��
	if (animPrev_.attachNo != -1)
	{
		MV1DetachAnim(modelHandle_, animPrev_.attachNo);
		InitAnimData(animPrev_);
	}

	// ���ݍĐ����Ă���A�j���[�V�������Â��A�j���Ƃ���
	animPrev_ = animNext_;

	// �V�����A�j���[�V������ݒ�
	animNext_.animNo = animNo;
	animNext_.playSpeed = animPlaySpeed;
	animNext_.attachNo = MV1AttachAnim(modelHandle_, animNo, -1, false);
	animNext_.totalTime = MV1GetAttachAnimTotalTime(modelHandle_, animNext_.attachNo);
	animNext_.isLoop = isLoop;

	// �ύX�ɂ�����t���[�������o���Ă���
	animChangeFrameTotal_ = changeFrame;
	animChangeFrame_ = 0;

	// �A�j���[�V�����̃u�����h����ݒ肷��
	UpdateAnimBlendRate();
}

// ���݂̃A�j���[�V�������I�����Ă��邩�ǂ������擾����
bool Model::IsAnimEnd()
{
	// Loopp�A�j���̏ꍇ��false��Ԃ�
	if (animNext_.isLoop)
	{
		return false;
	}

	// �A�^�b�`���Ă���A�j���[�V�����̍Đ����Ԃ��擾����
	float time = MV1GetAttachAnimTime(modelHandle_, animNext_.attachNo);

	// �A�j���[�V�������Đ��������Ԃ��A�j���[�V�����̑����Ԃ𒴂��Ă�����
	// �A�j���[�V�������I�����Ă���̂�true��Ԃ�
	if (time >= animNext_.totalTime)
	{
		return true;
	}
	return false;
}

// ���f���̃n���h���̎擾
int Model::GetModelHandle() const
{
	return modelHandle_;		
}

// �����蔻��Ɏg�p����t���[���C���f�b�N�X���擾����
int Model::GetColFrameIndex() const
{
	return colFrameIndex_;
}

// �A�j���[�V�����f�[�^�̏�����
void Model::InitAnimData(AnimData& anim)
{
	anim.animNo = -1;
	anim.attachNo = -1;
	anim.totalTime = 0.0f;
	anim.playSpeed = 1.0f;
	anim.isLoop = false;
}

// �A�j���[�V�����̍X�V
void Model::UpdateAnim(AnimData anim)
{
	// �A�j���[�V�������ݒ肳��Ă��Ȃ��ꍇ�͉������Ȃ�
	if (anim.attachNo == -1) return;

	// �A�j���[�V�����̍X�V
	float time = MV1GetAttachAnimTime(modelHandle_, anim.attachNo);
	time += anim.playSpeed;

	if (time > anim.totalTime)
	{
		if (anim.isLoop)
		{
			// �A�j���[�V�����̃��[�v
			time -= anim.totalTime;
		}
		else
		{
			// �Ō�̏�ԂŎ~�܂�
			time = anim.totalTime;
		}
	}
	// �A�^�b�`���Ă���A�j���[�V�����̍Đ����Ԃ�ݒ肷��
	MV1SetAttachAnimTime(modelHandle_, anim.attachNo, time);
}

// ���݂̃A�j���[�V�����؂�ւ���񂩂�A�j���[�V�����̃u�����h����ݒ肷��
void Model::UpdateAnimBlendRate()
{
	// �A�j���[�V�����ω��̃t���[�����ɉ������u�����h����ݒ肷��
	float rate = static_cast<float> (animChangeFrame_) / static_cast<float>(animChangeFrameTotal_);
	if (rate > 1.0f)
	{
		rate = 1.0f;
	}
	MV1SetAttachAnimBlendRate(modelHandle_, animPrev_.attachNo, 1.0f - rate);
	MV1SetAttachAnimBlendRate(modelHandle_, animNext_.attachNo, rate);
}