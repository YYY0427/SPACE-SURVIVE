#pragma once
#include <algorithm>
#include <cmath>
#include <cassert>

// �����l�Ə���l���Ǘ�����͈̓N���X
template<typename T>
class Range
{
public:
	/// <summary>
	/// �R���X�g���N�^
	/// �����l�Ə���l�̐ݒ�
	/// </summary>
	/// <param name="min">�����l</param>
	/// <param name="max">����l</param>
	Range(T min, T max) :
		min_(min),
		max_(max)
	{
		// �����l������l�̂ق����������ꍇ�~�߂�
		assert(min_ <= max_);
	}

	/// <summary>
	/// �l���ݒ肵�������l�Ə���l�͈͓̔����ǂ���
	/// </summary>
	/// <param name="value">�͈͓������ׂ����l</param>
	/// <returns>true : �͈͓�, false : �͈͓�����Ȃ�</returns>
	bool IsInside(T value) const
	{
		return (min_ <= value) && (value <= max_);
	}

	/// <summary>
	/// �l��ݒ肵���͈͓��ɃN�����v����
	/// </summary>
	/// <param name="value">�N�����v�������l</param>
	/// <returns>�N�����v��̒l</returns>
	T Clamp(T value) const
	{
		return  std::clamp(value, min_, max_);
	}

	T Wrap(T value) const
	{
		const float n = std::fmod(value - min_, max_ - min_);
		return (n >= 0) ? (n + min_) : (n + max_);
	}

	// �ݒ肵�������l�̎擾
	T GetMinValue() const { return min_; }

	// �ݒ肵������l�̎擾
	T GetMaxValue() const { return max_; }
private:
	// �����l
	T min_;

	// ����l
	T max_;
};