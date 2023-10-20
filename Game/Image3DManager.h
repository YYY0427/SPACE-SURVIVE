#pragma once
#include <map>
#include <memory>
#include <vector>
#include "Util/DataReaderFromUnity.h"

// �v���g�^�C�v�錾
class Image3D;

// 3D�摜�̊Ǘ��N���X
class Image3DManager
{
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="roadsData">Unity����ǂݍ��񂾓��̃f�[�^�̃e�[�u��</param>
	Image3DManager(std::vector<UnityGameObject> roadsData);

	// �f�X�g���N�^
	virtual ~Image3DManager();

	// �`��
	void Draw();

	// �S�Ă̓��̎擾
	std::vector<std::shared_ptr<Image3D>> GetRoads() const;

	// Y���������������S�Ă̓��̒�����󂯂��Ƃ����ʒu����1�ԋ߂����̍��W�̎擾
	VECTOR GetClosestRoadPos(VECTOR targetPos);

private:
	// 3D�摜�̎��
	enum class Image3DType
	{
		ROAD,
	};
private:
	std::vector<std::shared_ptr<Image3D>> pRoads_;

	// �摜�̃n���h�����i�[����e�[�u��
	std::map<Image3DType, int> handleTable_;
};