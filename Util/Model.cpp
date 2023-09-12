#include "Model.h"
#include <cassert>

namespace
{
	// �����蔻��Ƃ��Ďg�p����t���[���̖��O
	const string collision_frame_name = "Collision";
}

/// <summary>
/// �t�@�C�������w�肵�ă��[�h���s��
/// </summary>
/// <param name="fileName">���f���n���h���̃t�@�C����</param>
Model::Model(string fileName) :
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

/// <summary>
/// �w�肳�ꂽ�n���h���̃��f����Duplicate���Đ�������
/// </summary>
/// <param name="orgModel">���f���n���h��</param>
Model::Model(int orgModel) :
	isUseCollision_(false),
	isUpdateColision_(false),
	colFrameIndex_(-1),
	animChangeFrame_(0),
	animChangeFrameTotal_(0)
{
	// ���[�h�Ɏ��s�������f�����R�s�[���Ƃ��Ďw�肳��Ă���
	assert(modelHandle_ != -1);
	modelHandle_ = MV1DuplicateModel(orgModel);

	// �A�j���[�V�������̏�����
	InitAnimData(animPrev_);
	InitAnimData(animNext_);
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
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

/// <summary>
/// �����蔻��ݒ�
/// </summary>
/// <param name="isUse">�����蔻������悤���邩</param>
/// <param name="isNeedUpdate">�X�V���K�v���ǂ���</param>
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

/// <summary>
/// �X�V
/// </summary>
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

/// <summary>
///  �`��
/// </summary>
void Model::Draw()
{
	MV1DrawModel(modelHandle_);
}

/// <summary>
/// ���f���̕\���ʒu�̐ݒ�
/// </summary>
/// <param name="pos">�ʒu���</param>
void Model::SetPos(VECTOR pos)
{
	MV1SetPosition(modelHandle_, pos);
}

/// <summary>
/// ���f���̉�]��Ԃ̐ݒ�
/// </summary>
/// <param name="rot">��]���</param>
void Model::SetRot(VECTOR rot)
{
	MV1SetRotationXYZ(modelHandle_, rot);
}

/// <summary>
/// ���f���̊g�嗦�̐ݒ�
/// </summary>
/// <param name="scale">�g����</param>
void Model::SetScale(VECTOR scale)
{
	MV1SetScale(modelHandle_, scale);
}

/// <summary>
/// �A�j���[�V������ݒ肷��(�ς��Ɛ؂�ւ���)
/// </summary>
/// <param name="animNo">�ύX��A�j���[�V�����ԍ�</param>
/// <param name="isLoop">�A�j���[�V���������[�v�����邩</param>
/// <param name="isForceChange">���łɎw�肳�ꂽ�A�j�����Đ�����Ă���ꍇ���ύX���邩</param>
void Model::SetAnimation(int animNo, bool isLoop, bool isForceChange)
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
	animNext_.attachNo = MV1AttachAnim(modelHandle_, animNo, -1, false);
	animNext_.totalTime = MV1GetAttachAnimTotalTime(modelHandle_, animNext_.attachNo);
	animNext_.isLoop = isLoop;

	animChangeFrameTotal_ = 1;
	animChangeFrame_ = 1;
}

/// <summary>
/// �A�j���[�V������ω�������(���t���[�������Đ؂�ւ���)
/// </summary>
/// <param name="animNo">�A�j���[�V�����ԍ�</param>
/// <param name="isLoop">�A�j���[�V���������[�v�����邩</param>
/// <param name="isForceChange">���łɎw�肳�ꂽ�A�j�����Đ�����Ă���ꍇ���ύX���邩</param>
/// <param name="changeFrame">���t���[�������ăA�j���[�V������ύX�����邩</param>
void Model::ChangeAnimation(int animNo, bool isLoop, bool isForceChange, int changeFrame)
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
	animNext_.attachNo = MV1AttachAnim(modelHandle_, animNo, -1, false);
	animNext_.totalTime = MV1GetAttachAnimTotalTime(modelHandle_, animNext_.attachNo);
	animNext_.isLoop = isLoop;

	// �ύX�ɂ�����t���[�������o���Ă���
	animChangeFrameTotal_ = changeFrame;
	animChangeFrame_ = 0;

	// �A�j���[�V�����̃u�����h����ݒ肷��
	UpdateAnimBlendRate();
}

/// <summary>
/// // ���݂̃A�j���[�V�������I�����Ă��邩�ǂ������擾����(Loop�A�j���̏ꍇ�͎擾�ł��Ȃ�(false��Ԃ�))
/// </summary>
/// <returns>true : �I�� �A false : �Đ���</returns>
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

/// <summary>
/// ���f���̃n���h���̎擾
/// </summary>
/// <returns>���f���̃n���h��</returns>
int Model::GetModelHandle() const
{
	return modelHandle_;		
}

/// <summary>
/// �����蔻��Ɏg�p����t���[���C���f�b�N�X���擾����
/// </summary>
/// <returns>�����蔻��Ɏg�p����t���[���C���f�b�N�X</returns>
int Model::GetColFrameIndex() const
{
	return colFrameIndex_;
}

/// <summary>
/// �A�j���[�V�����f�[�^�̏�����
/// </summary>
/// <param name="anim">�A�j���[�V�����̃A�^�b�`�ԍ�</param>
void Model::InitAnimData(AnimData& anim)
{
	anim.animNo = -1;
	anim.attachNo = -1;
	anim.totalTime = 0.0f;
	anim.isLoop = false;
}

/// <summary>
/// �A�j���[�V�����̍X�V
/// </summary>
/// <param name="anim"></param>
/// <param name="dt"></param>
void Model::UpdateAnim(AnimData anim, float dt)
{
	// �A�j���[�V�������ݒ肳��Ă��Ȃ��ꍇ�͉������Ȃ�
	if (anim.attachNo == -1) return;

	// �A�j���[�V�����̍X�V
	float time = MV1GetAttachAnimTime(modelHandle_, anim.attachNo);
	time += dt;
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

/// <summary>
//  ���݂̃A�j���[�V�����؂�ւ���񂩂�A�j���[�V�����̃u�����h����ݒ肷��
/// </summary>
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