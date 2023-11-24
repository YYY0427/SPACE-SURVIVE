#pragma once
#include "LazerBase.h"
#include "../Util/Timer.h"

class NormalLazer : public LazerBase
{
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="modelHandle">���f���n���h��</param>
	/// <param name="firePos">���ˈʒu</param>
	/// <param name="vec">�x�N�g��(���K������Ă�)</param>
	/// <param name="fireObjectMoveVec">���ˌ��̈ړ��x�N�g��</param>
	/// <param name="isContinue">�p�����˂̃��[�U�[���ǂ���</param>
	NormalLazer(int modelHandle, VECTOR* firePos, VECTOR* vec, float effectSpeed, bool isContinue);

	// �f�X�g���N�^
	~NormalLazer();

	// �X�V
	void Update() override;

	// �`��
	void Draw() override;

	// ���[�U�[�̔���
	void Stop(const VECTOR pos) override;

	// ���[�U�[�̃G�t�F�N�g�̍Đ����I�����Ă����瓖���蔻��p�̃��f�����폜
	void ConfirmDelete() override;

	VECTOR GetVec() const override;

private:
	// ��]�s��
	MATRIX rotMtx_;

	// ���ˌ��̃x�N�g����ۑ�
	VECTOR* vec_;

	// �G�t�F�N�g�̈ʒu
	VECTOR effectPos_;

	// ���[�U�[�̃G�t�F�N�g�̃n���h��
	int laserEffectHandle_;

	// ���[�U�[�̓����蔻�胂�f���ƃG�t�F�N�g�̔��˃t���[���̍��𑪂�^�C�}�[
	Timer<int> collisionAndEffectDifferenceTimer_;

	bool isReflect_;
};

