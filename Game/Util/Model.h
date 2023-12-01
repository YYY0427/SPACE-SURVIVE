#pragma once
#include <DxLib.h>
#include <string>
#include <vector>

// TODO : �����蔻��֌W�̊֐��̒ǉ�

// ���f���̊Ǘ��N���X
class Model
{
public:
	/// <summary>
	/// �R���X�g���N�^
	/// �t�@�C�������w�肵�ă��[�h���s��
	/// </summary>
	/// <param name="fileName">���f���n���h���̃t�@�C����</param>
	Model(std::string fileName);		

	/// <summary>
	/// �R���X�g���N�^
	/// �w�肳�ꂽ�n���h���̃��f�����R�s�[���Đ�������
	/// </summary>
	/// <param name="orgModel">���f���n���h��</param>
	Model(int modelHandle);		

	// �f�X�g���N�^
	~Model();

	/// <summary>
	/// �����蔻��ݒ�
	/// </summary>
	/// <param name="isUse">�����蔻������悤���邩</param>
	/// <param name="isNeedUpdate">�X�V���K�v���ǂ���</param>
	void SetUseCollision(bool isUse, bool isNeedUpdate = true);

	// �X�V
	void Update();

	// �`��
	void Draw();

	/// <summary>
	/// �A�j���[�V������ݒ肷��(�ς��Ɛ؂�ւ���)
	/// </summary>
	/// <param name="animNo">�ύX��A�j���[�V�����ԍ�</param>
	/// <param name="isLoop">�A�j���[�V���������[�v�����邩</param>
	///	<param name="isForceChange">���łɎw�肳�ꂽ�A�j�����Đ�����Ă���ꍇ���ύX���邩</param>
	void SetAnimation(int animNo, bool isLoop, bool isForceChange, float animPlaySpeed = 1.0f);

	/// <summary>
	/// �A�j���[�V������ω�������(���t���[�������Đ؂�ւ���)
	/// </summary>
	/// <param name="animNo">�A�j���[�V�����ԍ�</param>
	/// <param name="isLoop">�A�j���[�V���������[�v�����邩</param>
	/// <param name="isForceChange">���łɎw�肳�ꂽ�A�j�����Đ�����Ă���ꍇ���ύX���邩</param>
	/// <param name="changeFrame">���t���[�������ăA�j���[�V������ύX�����邩</param>
	void ChangeAnimation(int animNo, bool isLoop, bool isForceChange, int changeFrame, float animPlaySpeed = 1.0f);

	/// <summary>
	/// ���݂̃A�j���[�V�������I�����Ă��邩�ǂ������擾����
	/// Loop�A�j���̏ꍇ�͎擾�ł��Ȃ�(false��Ԃ�)
	/// </summary>
	/// <returns>true : �I���Afalse : �Đ���</returns>
	bool IsAnimEnd();

	// �A�j���[�V�������~�߂�
	void StopAnim();

	// �Q�b�^�[
	int GetModelHandle() const;		// ���f���̃n���h���̎擾
	int GetColFrameIndex() const;	// �����蔻��Ɏg�p����t���[���C���f�b�N�X���擾����
	int GetMaterialNum() const;		// ���f���̃}�e���A���̐��̎擾

	// ���f���ݒ�
	void SetPos(VECTOR pos);			// �\���ʒu�̐ݒ�
	void SetRot(VECTOR rot);			// ��]��Ԃ̐ݒ�
	void SetRotMtx(MATRIX mtx);			// ��]�s��̐ݒ�
	void SetScale(VECTOR scale);		// �g�嗦�̐ݒ�
	void SetOpacity(float opacity);		// �s�����x�̐ݒ�(0.0~1.0)

	// �`�惂�[�h�ݒ�
	void SetMaterialDrawBlendMode(int materialIndex, int blendMode);	// �w��̃}�e���A���̕`�惂�[�h�̐ݒ�
	void SetAllMaterialDrawBlendMode(int blendMode);					// �S�Ẵ}�e���A���̕`�惂�[�h�̐ݒ�
	void SetMaterialDrawBlendParam(int materialIndex, int blendParam);	// �w��̃}�e���A���̕`��u�����h�p�����[�^�̐ݒ�
	void SetAllMaterialDrawBlendParam(int blendParam);					// �S�Ẵ}�e���A���̕`��u�����h�p�����[�^�̐ݒ�

	// �f�B�t���[�Y�J���[�ݒ�
	void SetMaterialDifColor(int materialIndex, COLOR_F color);	// �w��̃}�e���A���̃f�B�t���[�Y�J���[�̐ݒ�
	void SetAllMaterialDifColor(COLOR_F color);					// �S�Ẵ}�e���A���̃f�B�t���[�Y�J���[�̐ݒ�
	void InversAllMaterialDifColor();							// �S�Ẵ}�e���A���̃f�B�t���[�Y�J���[�𔽓]
	void RestoreAllMaterialDifColor();							// �S�Ẵ}�e���A���̃f�B�t���[�Y�J���[�����ɖ߂�


private:
	// �A�j���[�V�������
	struct AnimData
	{
		int animNo;			// �A�j���[�V�����ԍ�
		int attachNo;		// �A�^�b�`�ԍ�
		float totalTime;	// �A�j���[�V�����̑��Đ�����
		float playSpeed;	// �A�j���[�V�����̍Đ����x
		bool isLoop;		// �A�j���[�V���������[�v���邩
	};

private:
	/// <summary>
	/// �A�j���[�V�����f�[�^�̏�����
	/// </summary>
	/// <param name="anim">�A�j���[�V�����̃A�^�b�`�ԍ�</param>
	void InitAnimData(AnimData& anim);

	/// <summary>
	/// �A�j���[�V�����̍X�V
	/// </summary>
	/// <param name="anim">�A�j���[�V�����̃A�^�b�`�ԍ�</param>
	void UpdateAnim(AnimData anim);

	// ���݂̃A�j���[�V�����؂�ւ���񂩂�A�j���[�V�����̃u�����h����ݒ肷��
	void UpdateAnimBlendRate();
private:
	// ���f���̃n���h��
	int modelHandle_;

	// �����蔻������g�p����
	bool isUseCollision_;

	// �����蔻����𖈃t���[���X�V����
	bool isUpdateColision_;

	// �����蔻��Ƃ��Ďg�p����t���[���̃C���f�b�N�X
	int colFrameIndex_;

	// �A�j���[�V�����̃A�^�b�`�ԍ�
	AnimData animPrev_;		// �ύX�O�A�j���[�V�������
	AnimData animNext_;		// �ύX��A�j���[�V�������

	// �A�j���[�V�����̐؂�ւ����
	int animChangeFrame_;			// ���݂̐؂�ւ��t���[����
	int animChangeFrameTotal_;		// �؂�ւ��ɂ����鑍�t���[����

	// �S�Ă̌��̃}�e���A���̃f�B�t���[�Y�J���[��ۑ����Ă���e�[�u��
	std::vector<COLOR_F> matarialColorTable_;
};