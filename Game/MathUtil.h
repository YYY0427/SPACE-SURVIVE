#pragma once
#include <DxLib.h>

// ��]���� 
enum class RotationOrder
{
	XYZ,			// X �� Y �� Z 
	YZX,			// Y �� Z �� X 
	ZXY,			// Z �� X �� Y 
};

namespace MathUtil
{
	/// <summary>
	/// ��]�s��� XYZ �e���̃I�C���[�p�x�ɕϊ�
	/// </summary>
	/// <param name="order">��]����</param>
	/// <param name="mtx">�ϊ��������s��</param>
	/// <param name="isGimbalLock">�W���o�����b�N�̔����L�����i�[����ϐ�</param>
	/// <returns>XYZ �e���̃I�C���[�p�x (���W�A���P��) ���i�[�����x�N�g��</returns>
	VECTOR ToEulerAngles(const MATRIX mtx, bool& isGimbalLock, RotationOrder order = RotationOrder::ZXY);
}