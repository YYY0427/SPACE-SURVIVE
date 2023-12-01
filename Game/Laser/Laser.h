#pragma once
#include "LaserBase.h"
#include "../Util/Timer.h"

class Laser : public LaserBase
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
	Laser(int modelHandle, VECTOR* firePos, VECTOR* vec, float fireFrameCount, bool isInfinity, int chargeFrame);

	// �f�X�g���N�^
	~Laser();

	// �X�V
	void Update() override;

	// �`��
	void Draw() override;

	// ���[�U�[�̔���
	void Stop(const VECTOR pos);

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
	Timer<int> chargeEffectTimer_;
	
	// ���[�U�[�����t���[���c�葱���邩�𑪂�^�C�}�[
	Timer<int> fireFrameTimer_;

	// ���˂������邩
	bool isInfinity_;

	bool isReflect_;
};

