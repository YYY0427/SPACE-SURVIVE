#pragma once
#include <map>
#include <memory>
#include <vector>
#include "Util/DataReaderFromUnity.h"

// �v���g�^�C�v�錾
class Road;

// 3D�摜�̊Ǘ��N���X
class RoadManager
{
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="roadsData">Unity����ǂݍ��񂾓��̃f�[�^�̃e�[�u��</param>
	RoadManager(std::vector<UnityGameObject> roadsData);

	// �f�X�g���N�^
	virtual ~RoadManager();

	/// <summary>
	/// �X�V
	/// </summary>
	/// <param name="playerPos">�v���C���[�̍��W</param>
	void Update(VECTOR playerPos);

	// �`��
	void Draw();

	// ���̎擾
	std::vector<std::shared_ptr<Road>> GetRoads() const;

	// Y���������������S�Ă̓��̒�����󂯂��Ƃ����ʒu����1�ԋ߂����̍��W�̎擾
	VECTOR GetClosestRoadPos(VECTOR targetPos);

private:
	// 3D�摜�̎��
	enum class Image3DType
	{
		ROAD,
	};
private:
	std::vector<std::shared_ptr<Road>> pRoads_;
};