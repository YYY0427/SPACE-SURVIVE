#pragma once
#include <DxLib.h>

// ��]���� 
enum class RotationOrder
{
	XYZ,			// X �� Y �� Z 
	YZX,			// Y �� Z �� X 
	ZXY,			// Z �� X �� Y 
};

class MathUtil
{
public:
	/// <summary>
	/// ��]�s���XYZ �e���̃I�C���[�p�x�ɕϊ�
	/// </summary>
	/// <param name="order">��]����</param>
	/// <param name="mtx">�ϊ��������s��</param>
	/// <param name="isGimbalLock">�W���o�����b�N�̔����L�����i�[����ϐ� true : �W���o�����b�N����</param>
	/// <returns>XYZ �e���̃I�C���[�p�x (���W�A���P��) ���i�[�����x�N�g��</returns>
	static VECTOR ToEulerAngles(const MATRIX mtx, bool& isGimbalLock, RotationOrder order = RotationOrder::ZXY);

	/// <summary>
	/// �x���@����ʓx�@�ɕϊ�(180->��)
	/// </summary>
	/// <param name="degree">�p�x</param>
	/// <returns>���W�A��</returns>
	static float RadianFromDegree(float radian);

	/// <summary>
	/// �ʓx�@����x���@�ɕϊ�(��->180)
	/// </summary>
	/// <param name="degree">���W�A��</param>
	/// <returns>�p�x</returns>
	static float DegreeFromRadian(float degree);

private:
	static bool EulerAnglesXYZ(const MATRIX mat, float& xRot, float& yRot, float& zRot);
	static bool EulerAnglesYZX(const MATRIX mat, float& xRot, float& yRot, float& zRot);
	static bool EulerAnglesZXY(const MATRIX mat, float& xRot, float& yRot, float& zRot);
};