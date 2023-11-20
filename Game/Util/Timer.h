#pragma once
#include <algorithm>

// タイマークラス
template<class T>
class Timer
{
public: 
	// コンストラクタ
	Timer() :
		time_(0),
		limitTime_(0)
	{
	}

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="limitTime">制限時間</param>
	Timer(T limitTime) :
		time_(0),
		limitTime_(limitTime)
	{
	}

	// デストラクタ
	virtual ~Timer() {};

	/// <summary>
	/// タイマーの更新
	/// </summary>
	/// <param name="deltaTime">タイマーを1フレーム毎に更新する値</param>
	void Update(T deltaTime)
	{
		// 更新
		time_ += deltaTime;

		if (limitTime_ > 0)
		{
			// 制限時間は超えない
			time_ = (std::min)(time_, limitTime_);
		}
	}

	// タイマーの初期化
	void Reset()
	{
		time_ = 0;
	}

	/// <summary>
	/// タイムアウトかどうか
	/// 制限時間を設定していない場合、常にfalseを返す
	/// </summary>
	/// <returns>true : タイムアウト, false : タイム内</returns>
	bool IsTimeOut() const
	{
		// 制限時間を設定していない場合、タイム内を常に返す
		if (limitTime_ <= 0)
		{
			return false;
		}

		return time_ >= limitTime_;
	}

	/// <summary>
	/// タイムの取得
	/// </summary>
	/// <returns>タイム</returns>
	T GetTime() const { return time_; }

	/// <summary>
	/// 制限時間の設定
	/// </summary>
	/// <param name="limitTime">制限時間</param>
	void SetLimitTime(T limitTime) { limitTime_ = limitTime; }

private:
	// 現在の時間
	T time_;

	// 制限時間
	T limitTime_;
};