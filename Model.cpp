#include "Model.h"
#include <cassert>

namespace
{
	// �����蔻��Ƃ��Ďg�p����t���[���̖��O
	const char* const collision_frame_name = "Collision";
}

Model::Model(const char* fileName) :
	m_isUseCollision(false),
	m_isUpdateColision(false),
	m_colFrameIndex(-1),
	m_animChangeFrame(0),
	m_animChangeFrameTotal(0)
{
	m_modelHandle = MV1LoadModel(fileName);

	// ���f���̃��[�h�Ɏ��s����
	assert(m_modelHandle != -1);

	// �A�j���[�V�������̃N���A
	ClearAnimData(m_animPrev);
	ClearAnimData(m_animNext);
}

Model::Model(int orgModel) :
	m_isUseCollision(false),
	m_isUpdateColision(false),
	m_colFrameIndex(-1),
	m_animChangeFrame(0),
	m_animChangeFrameTotal(0)
{
	// ���[�h�Ɏ��s�������f�����R�s�[���Ƃ��Ďw�肳��Ă���
	assert(m_modelHandle != -1);
	m_modelHandle = MV1DuplicateModel(orgModel);

	// �A�j���[�V�������̃N���A
	ClearAnimData(m_animPrev);
	ClearAnimData(m_animNext);
}

Model::~Model()
{
	if (m_isUpdateColision)
	{
		// �����蔻������g�p���Ă�����j������
		MV1TerminateCollInfo(m_modelHandle, -1);
		m_isUseCollision = false;
	}
	MV1DeleteModel(m_modelHandle);
}

void Model::SetUseCollision(bool isUse, bool isNeedUpdate)
{
	// �����蔻����g��Ȃ��̂ɓ����蔻��̍X�V�𖈃t���[�������Ȃ������ꍇ���͂���
	assert(isUse || !isNeedUpdate);

	// �����蔻��g�p���ɕω����������ꍇ�̂ݍX�V����
	if (m_isUpdateColision != isUse)
	{
		if (isUse)
		{
			// �g��Ȃ����g��
			// �����蔻��p�t���[��������
			m_colFrameIndex = MV1SearchFrame(m_modelHandle, collision_frame_name);

			if (m_colFrameIndex < 0)	// ������Ȃ����� or �G���[
			{
				m_colFrameIndex = -1;
			}
			MV1SetupCollInfo(m_modelHandle, m_colFrameIndex, 8, 8, 8);
		}
		else
		{
			// �g�����g��Ȃ�
			MV1TerminateCollInfo(m_modelHandle, -1);
		}
	}

	m_isUseCollision = isUse;
	m_isUpdateColision = isNeedUpdate;
}

void Model::Update()
{
	UpdateAnim(m_animPrev);
	UpdateAnim(m_animNext);

	// �w��t���[�������ăA�j���[�V������ύX����
	m_animChangeFrame++;
	if (m_animChangeFrame > m_animChangeFrameTotal)
	{
		m_animChangeFrame = m_animChangeFrameTotal;
	}

	// �A�j���[�V�����̃u�����h����ݒ肷��
	UpdateAnimBlendRate();

	// �����蔻��f�[�^�̍X�V
	if (m_isUseCollision && m_isUpdateColision)
	{
		MV1RefreshCollInfo(m_modelHandle, m_colFrameIndex);
	}
}

void Model::Draw()
{
	MV1DrawModel(m_modelHandle);
}

void Model::SetPos(VECTOR pos)
{
	MV1SetPosition(m_modelHandle, pos);
}

void Model::SetRot(VECTOR rot)
{
	MV1SetRotationXYZ(m_modelHandle, rot);
}

void Model::SetScale(VECTOR scale)
{
	MV1SetScale(m_modelHandle, scale);
}

void Model::SetAnimation(int animNo, bool isLoop, bool isForceChange)
{
	if (!isForceChange)
	{
		// ���łɍĐ�����Ă���A�j���͍Đ����Ȃ�
		if (m_animNext.animNo == animNo)	return;
	}

	// �ȑO�̃A�j���[�V�������c���Ă���ΏI��
	if (m_animPrev.attachNo != -1)
	{
		MV1DetachAnim(m_modelHandle, m_animPrev.attachNo);
		ClearAnimData(m_animPrev);
	}
	if (m_animNext.attachNo != -1)
	{
		MV1DetachAnim(m_modelHandle, m_animNext.attachNo);
		ClearAnimData(m_animNext);
	}
	// �V�����A�j���[�V������ݒ�
	m_animNext.animNo = animNo;
	m_animNext.attachNo = MV1AttachAnim(m_modelHandle, animNo, -1, false);
	m_animNext.totalTime = MV1GetAttachAnimTotalTime(m_modelHandle, m_animNext.attachNo);
	m_animNext.isLoop = isLoop;

	m_animChangeFrameTotal = 1;
	m_animChangeFrame = 1;
}

void Model::ChangeAnimation(int animNo, bool isLoop, bool isForceChange, int changeFrame)
{
	if (!isForceChange)
	{
		// ���łɍĐ�����Ă���A�j���͍Đ����Ȃ�
		if (m_animNext.animNo == animNo)	return;
	}

	// �ȑO�̃A�j���[�V�������c���Ă���ΏI��
	if (m_animPrev.attachNo != -1)
	{
		MV1DetachAnim(m_modelHandle, m_animPrev.attachNo);
		ClearAnimData(m_animPrev);
	}
	// ���ݍĐ����Ă���A�j���[�V�������Â��A�j���Ƃ���
	m_animPrev = m_animNext;

	// �V�����A�j���[�V������ݒ�
	m_animNext.animNo = animNo;
	m_animNext.attachNo = MV1AttachAnim(m_modelHandle, animNo, -1, false);
	m_animNext.totalTime = MV1GetAttachAnimTotalTime(m_modelHandle, m_animNext.attachNo);
	m_animNext.isLoop = isLoop;

	// �ύX�ɂ�����t���[�������o���Ă���
	m_animChangeFrameTotal = changeFrame;
	m_animChangeFrame = 0;

	// �A�j���[�V�����̃u�����h����ݒ肷��
	UpdateAnimBlendRate();
}

bool Model::IsAnimEnd()
{
	// Loopp�A�j���̏ꍇ�͏��false��Ԃ�
	if (m_animNext.isLoop) return false;

	float time = MV1GetAttachAnimTime(m_modelHandle, m_animNext.attachNo);

	if (time >= m_animNext.totalTime) return true;

	return false;
}

void Model::ClearAnimData(AnimData& anim)
{
	anim.animNo = -1;
	anim.attachNo = -1;
	anim.totalTime = 0.0f;
	anim.isLoop = false;
}

void Model::UpdateAnim(AnimData anim, float dt)
{
	// �A�j���[�V�������ݒ肳��Ă��Ȃ��ꍇ�͉������Ȃ�
	if (anim.attachNo == -1) return;

	// �A�j���[�V�����̍X�V
	float time = MV1GetAttachAnimTime(m_modelHandle, anim.attachNo);
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
	MV1SetAttachAnimTime(m_modelHandle, anim.attachNo, time);
}

void Model::UpdateAnimBlendRate()
{
	// �A�j���[�V�����ω��̃t���[�����ɉ������u�����h����ݒ肷��
	float rate = static_cast<float> (m_animChangeFrame) / static_cast<float>(m_animChangeFrameTotal);
	if (rate > 1.0f)	rate = 1.0f;
	MV1SetAttachAnimBlendRate(m_modelHandle, m_animPrev.attachNo, 1.0f - rate);
	MV1SetAttachAnimBlendRate(m_modelHandle, m_animNext.attachNo, rate);
}