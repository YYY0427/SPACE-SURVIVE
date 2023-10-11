#include "Image3DManager.h"
#include "Image3D.h"
#include "Util/DrawFunctions.h"
#include <string>

namespace
{
	// ���̉摜�̃t�@�C���p�X
	const std::string load_img_file_path = "Data/Image/Road.png";
}

// �R���X�g���N�^
Image3DManager::Image3DManager(std::vector<UnityGameObject> roadsData)
{
	// �摜�̃��[�h
	handleTable_[Image3DType::LOAD] = my::MyLoadGraph(load_img_file_path.c_str());

	// �󂯂��Ƃ����f�[�^�̐��C���X�^���X�̍쐬
	for (auto& roadData : roadsData)
	{
		pRoads_.push_back(std::make_shared<Image3D>(handleTable_[Image3DType::LOAD], roadData));
	}
}

// �f�X�g���N�^
Image3DManager::~Image3DManager()
{
	// �摜�����ׂč폜
	for (auto& handle : handleTable_)
	{
		DeleteGraph(handle.second);
	}
}

// �`��
void Image3DManager::Draw()
{
	// ���̕`��
	for (auto& road : pRoads_)
	{
		road->Draw();
	}
}

// �S�Ă̓��̎擾
std::vector<std::shared_ptr<Image3D>> Image3DManager::GetRoads() const
{
	return pRoads_;
}

// Y���������������S�Ă̓��̒�����󂯂��Ƃ����ʒu����1�ԋ߂����̍��W�̎擾
VECTOR Image3DManager::GetClosestRoadPos(VECTOR targetPos)
{
	// 1�ԃ^�[�Q�b�g�܂ŋ߂���
	VECTOR closestPos = VGet(0.0f, 0.0f, 0.0f);

	// �^�[�Q�b�g�܂�1�ԋ߂��傫��
	float nearDistanceSize = 0.0f;

	// 1�񂾂��ʂ��悤�̕ϐ�
	bool isFirst = false;

	for (auto& road : pRoads_)
	{
		// Y������������
		targetPos = VGet(targetPos.x, 0.0f, targetPos.z);
		VECTOR roadPos = VGet(road->GetPos().x, 0.0f, road->GetPos().z);

		// ������^�[�Q�b�g�܂ł̋����̑傫�������߂�
		float distanceSize = VSize(VSub(roadPos, targetPos));

		// �^�[�Q�b�g�܂�1�ԋ߂��傫���ƌ��݂̓�����^�[�Q�b�g�܂ł̋����̑傫�����ׂ�
		// ���݂̓��̋����ق�����������������̍X�V
		// 1�ԍŏ��̓��͔�ׂ�傫�����Ȃ��̂Œʂ�
		if (nearDistanceSize > distanceSize || !isFirst)
		{
			// 1��ʂ����̂Ńt���O�𗧂Ă�
			isFirst = true;

			// 1�ԃ^�[�Q�b�g�܂ŋ߂��傫���̍X�V
			nearDistanceSize = distanceSize;

			// 1�ԃ^�[�Q�b�g�܂ŋ߂����̍X�V
			closestPos = VGet(road->GetPos().x, road->GetPos().y, road->GetPos().z);
		}
	}
	return closestPos;
}