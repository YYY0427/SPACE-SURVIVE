#include "Timer.h"
#include <algorithm>

// �R���X�g���N�^
Timer::Timer(int limitTime) :
	time_(0),
	limitTime_(limitTime)
{
}

// �f�X�g���N�^
Timer::~Timer()
{
	// �����Ȃ�
}

// ���Ԃ̍X�V
void Timer::Update(int deltaTime)
{
	// �������Ԃ͒����Ȃ�
	time_ = std::min(time_ + deltaTime, limitTime_);
}

// ���Ԃ̏�����
void Timer::Reset()
{
	time_ = 0;
}

// �^�C���A�E�g�������ǂ���
bool Timer::IsTimeOut() const
{
	return time_ >= limitTime_;
}
