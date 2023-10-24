#include "RoadManager.h"
#include "Road.h"
#include "Util/DrawFunctions.h"
#include <string>

// �R���X�g���N�^
RoadManager::RoadManager(std::vector<UnityGameObject> roadsData)
{
	// �󂯂��Ƃ����f�[�^�̐��C���X�^���X�̍쐬
	for (auto& roadData : roadsData)
	{
		pRoads_.push_back(std::make_shared<Road>(roadData));
	}
}

// �f�X�g���N�^
RoadManager::~RoadManager()
{
}
// �`��
void RoadManager::Draw()
{
	// ���̕`��
	for (auto& road : pRoads_)
	{
		road->Draw();
	}
}

// �S�Ă̓��̎擾
std::vector<std::shared_ptr<Road>> RoadManager::GetRoads() const
{
	return pRoads_;
}

// Y���������������S�Ă̓��̒�����󂯂��Ƃ����ʒu����1�ԋ߂����̍��W�̎擾
VECTOR RoadManager::GetClosestRoadPos(VECTOR targetPos)
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
		VECTOR roadPos = VGet(road->GetImage3D()->GetPos().x, 0.0f, road->GetImage3D()->GetPos().z);

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
			closestPos = VGet(road->GetImage3D()->GetPos().x, road->GetImage3D()->GetPos().y, road->GetImage3D()->GetPos().z);
		}
	}
	return closestPos;
}