#pragma once
#include <algorithm>
#include <cmath>
#include <cassert>

// 下限値と上限値を管理する範囲クラス
template<class T>
class Range
{
public:
	/// <summary>
	/// コンストラクタ
	/// 下限値と上限値の設定
	/// </summary>
	/// <param name="min">下限値</param>
	/// <param name="max">上限値</param>
	Range(T min, T max) :
		min_(min),
		max_(max)
	{
		// 下限値より上限値のほうが小さい場合止める
		assert(min_ <= max_);
	}

	/// <summary>
	/// 値が設定した下限値と上限値の範囲内かどうか
	/// </summary>
	/// <param name="value">範囲内か調べたい値</param>
	/// <returns>true : 範囲内, false : 範囲内じゃない</returns>
	bool IsInside(T value) const
	{
		return (min_ <= value) && (value <= max_);
	}

	/// <summary>
	/// 値を設定した範囲内にクランプする
	/// </summary>
	/// <param name="value">クランプしたい値</param>
	/// <returns>クランプ後の値</returns>
	T Clamp(T value) const
	{
		return  std::clamp(value, min_, max_);
	}

	T Wrap(T value) const
	{
		const float n = std::fmod(value - min_, max_ - min_);
		return (n >= 0) ? (n + min_) : (n + max_);
	}

	// 設定した下限値の取得
	T GetMinValue() const { return min_; }

	// 設定した上限値の取得
	T GetMaxValue() const { return max_; }
private:
	// 下限値
	T min_;

	// 上限値
	T max_;
};