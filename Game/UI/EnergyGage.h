#pragma once

// �V�[���h�̃G�l���M�[�Q�[�W�N���X
class EnergyGage
{
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="maxEnergy">�G�l���M�[�ő�e��</param>
	EnergyGage(float maxEnergy);

	// �f�X�g���N�^
	~EnergyGage();

	/// <summary>
	/// �G�l���M�[�Q�[�W�̕`��
	/// </summary>
	/// <param name="x">�`�悵����x���W</param>
	/// <param name="y">�`�悵����y���W</param>
	/// <param name="energy">���݂̃G�l���M�[�c��</param>
	void Draw(int x, int y, float energy);

private:
	// �G�l���M�[�̍ő�e��
	float maxEnergy_;

	// �G�l���M�[�Q�[�W�̉摜�n���h��
	int energyBaseImgH_;
	int energyBarImgH_;
};