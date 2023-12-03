#pragma once
#include <algorithm>

// �^�C�}�[�N���X
template<class T>
class Timer
{
public: 
	// �R���X�g���N�^
	Timer() :
		time_(0),
		limitTime_(0)
	{
	}

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="limitTime">��������</param>
	Timer(T limitTime) :
		time_(0),
		limitTime_(limitTime)
	{
	}

	// �f�X�g���N�^
	virtual ~Timer() {};

	/// <summary>
	/// �^�C�}�[�̍X�V
	/// </summary>
	/// <param name="deltaTime">�^�C�}�[��1�t���[�����ɍX�V����l</param>
	void Update(T deltaTime)
	{
		// �X�V
		time_ += deltaTime;

		if (limitTime_ > 0)
		{
			// �������Ԃ͒����Ȃ�
			time_ = (std::min)(time_, limitTime_);
		}
	}

	// �^�C�}�[�̏�����
	void Reset()
	{
		time_ = 0;
	}

	/// <summary>
	/// �^�C���A�E�g���ǂ���
	/// �������Ԃ�ݒ肵�Ă��Ȃ��ꍇ�A���false��Ԃ�
	/// </summary>
	/// <returns>true : �^�C���A�E�g, false : �^�C����</returns>
	bool IsTimeOut() const
	{
		// �������Ԃ�ݒ肵�Ă��Ȃ��ꍇ�A�^�C���A�E�g����ɕԂ�
		if (limitTime_ <= 0)
		{
			return true;
		}

		return time_ >= limitTime_;
	}

	/// <summary>
	/// �^�C���̎擾
	/// </summary>
	/// <returns>�^�C��</returns>
	T GetTime() const { return time_; }

	/// <summary>
	/// �^�C���̐ݒ�
	/// </summary>
	/// <param name="limitTime">�ݒ肵�����^�C��</param>
	void SetTime(T time) { time_ = time; }

	// �������Ԃ̎擾
	T GetLimitTime() const { return limitTime_; }

private:
	// ���݂̎���
	T time_;

	// ��������
	T limitTime_;
};