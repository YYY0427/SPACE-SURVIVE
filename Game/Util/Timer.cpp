#include "Timer.h"
#include <algorithm>

// コンストラクタ
Timer::Timer(int limitTime) :
	time_(0),
	limitTime_(limitTime)
{
}

// デストラクタ
Timer::~Timer()
{
	// 処理なし
}

// 時間の更新
void Timer::Update(int deltaTime)
{
	// 制限時間は超えない
	time_ = std::min(time_ + deltaTime, limitTime_);
}

// 時間の初期化
void Timer::Reset()
{
	time_ = 0;
}

// タイムアウトしたかどうか
bool Timer::IsTimeOut() const
{
	return time_ >= limitTime_;
}
