#pragma once

// タイマークラス
class Timer
{
public: 
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="limitTime">制限時間</param>
	Timer(int limitTime);

	// デストラクタ
	virtual ~Timer();

	/// <summary>
	/// タイマーの更新
	/// </summary>
	/// <param name="deltaTime">タイマーを1フレーム毎に更新する値</param>
	void Update(int deltaTime);

	// タイマーの初期化
	void Reset();

	/// <summary>
	/// タイムアウトかどうか
	/// </summary>
	/// <returns>true : タイムアウト, false : タイム内</returns>
	bool IsTimeOut() const;

private:
	// 現在の時間
	int time_;

	// 制限時間
	int limitTime_;
};