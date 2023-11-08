#include "EnergyGage.h"
#include "../Util/DrawFunctions.h"
#include <DxLib.h>
#include <string>

namespace
{
	// �G�l���M�[�Q�[�W�̊g�嗦
	constexpr double energy_gagu_scale = 0.3;

	// �G�l���M�[�Q�[�W�̉摜�̃t�@�C���̃p�X
	const std::string energy_base_img_file_path = "Data/Image/EnergyBase.png";
	const std::string energy_bar_img_file_path = "Data/Image/EnergyBar.png";
}

// �R���X�g���N�^
EnergyGage::EnergyGage(float maxEnergy) :
	maxEnergy_(maxEnergy)
{
	energyBaseImgH_ = my::MyLoadGraph(energy_base_img_file_path.c_str());
	energyBarImgH_ = my::MyLoadGraph(energy_bar_img_file_path.c_str());
}

// �f�X�g���N�^
EnergyGage::~EnergyGage()
{
	DeleteGraph(energyBaseImgH_);
	DeleteGraph(energyBarImgH_);
}

// �G�l���M�[�Q�[�W�̕`��
void EnergyGage::Draw(int x, int y, float energy)
{
	// �G�l���M�[�Q�[�W�̘g�̕`��
	DrawRotaGraph(x, y, energy_gagu_scale, 0.0, energyBaseImgH_, true);
	
	// �G�l���M�[���Ȃ��ꍇ�̓G�l���M�[�o�[�̕`����s��Ȃ�
	if (energy > 0.0f)
	{
		// �G�l���M�[�o�[�̉摜�̃T�C�Y�̎擾
		float width, height;
		GetGraphSizeF(energyBarImgH_, &width, &height);

		// DrawRectRotaGraphF�̐�����A�w��̋�`�̕����̕`�悪�w�肵�����W�𒆐S�ɍs���A�摜������邽�߁A
		// �摜������Ȃ����������v�Z
		float ratio = (width - (width * (energy / maxEnergy_))) * 0.5f * energy_gagu_scale;

		// �G�l���M�[�o�[���G�l���M�[�̎c�ʂɂ���ĕ`��
		DrawRectRotaGraphF(x - ratio, y, 0, 0, width * (energy / maxEnergy_), height, energy_gagu_scale, 0.0, energyBarImgH_, true);
	}
}
