#pragma once
#include <DxLib.h>

/// <summary>
/// ���f���̊Ǘ��N���X
/// </summary>
class Model
{
public:
	// �R���X�g���N�^
	Model(const char* fileName);	// �t�@�C�������w�肵�ă��[�h���s��
	Model(int orgModel);			// �w�肳�ꂽ�n���h���̃��f����Duplicate���Đ�������

	// �f�X�g���N�^
	virtual ~Model();

	// �����蔻��ݒ�
	void SetUseCollision(bool isUse, bool isNeedUpdate = true);

	// �X�V
	void Update();

	// �`��
	void Draw();

	// �A�j���[�V������ݒ肷��(�ς��Ɛ؂�ւ���)
	void SetAnimation(int animNo, bool isLoop, bool isForceChange);

	// �A�j���[�V������ω�������(���t���[�������Đ؂�ւ���)
	void ChangeAnimation(int animNo, bool isLoop, bool isForceChange, int changeFrame);

	// ���݂̃A�j���[�V�������I�����Ă��邩�ǂ������擾����(Loop�A�j���̏ꍇ�͎擾�ł��܂��� false��Ԃ�)
	bool IsAnimEnd();

	// �Q�b�^�[
	int GetModelHandle() const { return modelHandle_; }		// ���f���̃n���h���̎擾
	int GetColFrameIndex() const { return m_colFrameIndex; }	// �����蔻��Ɏg�p����t���[���C���f�b�N�X���擾����

	// �Z�b�^�[
	void SetPos(VECTOR pos);		// �\���ʒu�̐ݒ�
	void SetRot(VECTOR rot);		// ��]��Ԃ̐ݒ�
	void SetScale(VECTOR scale);	// �g��l�̐ݒ�

private:
	// �A�j���[�V�������
	struct AnimData
	{
		int animNo;			// �A�j���[�V�����ԍ�
		int attachNo;		// �A�^�b�`�ԍ�
		float totalTime;	// �A�j���[�V�����̑��Đ�����
		bool isLoop;		// �A�j���[�V���������[�v���邩
	};

private:
	// �A�j���[�V�������̃N���A
	void ClearAnimData(AnimData& anim);

	// �A�j���[�V�����̍X�V
	void UpdateAnim(AnimData anim, float dt = 1.0f);

	// ���݂̃A�j���[�V�����؂�ւ���񂩂�A�j���[�V�����̃u�����h����ݒ肷��
	void UpdateAnimBlendRate();
private:
	// ���f���̃n���h��
	int modelHandle_;

	// �����蔻������g�p����
	bool m_isUseCollision;

	// �����蔻����𖈃t���[���X�V����
	bool m_isUpdateColision;

	// �����蔻��Ƃ��Ďg�p����t���[���̃C���f�b�N�X
	int m_colFrameIndex;

	// �A�j���[�V�����̃A�^�b�`�ԍ�
	AnimData animPrev_;	// �ύX�O�A�j���[�V�������
	AnimData animNext_;	// �ύX��A�j���[�V�������

	// �A�j���[�V�����̐؂�ւ����
	int animChangeFrame_;			// ���݂̐؂�ւ��t���[����
	int animChangeFrameTotal_;		// �؂�ւ��ɂ����鑍�t���[����
};