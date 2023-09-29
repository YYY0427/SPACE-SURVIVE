#pragma once

// �^�C�}�[�N���X
class Timer
{
public: 
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="limitTime">��������</param>
	Timer(int limitTime);

	// �f�X�g���N�^
	virtual ~Timer();

	/// <summary>
	/// �^�C�}�[�̍X�V
	/// </summary>
	/// <param name="deltaTime">�^�C�}�[��1�t���[�����ɍX�V����l</param>
	void Update(int deltaTime);

	// �^�C�}�[�̏�����
	void Reset();

	/// <summary>
	/// �^�C���A�E�g���ǂ���
	/// </summary>
	/// <returns>true : �^�C���A�E�g, false : �^�C����</returns>
	bool IsTimeOut() const;

private:
	// ���݂̎���
	int time_;

	// ��������
	int limitTime_;
};