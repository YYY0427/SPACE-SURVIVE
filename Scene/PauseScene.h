#pragma once
#include "Scene.h"

/// <summary>
/// �|�[�Y�V�[��
/// </summary>
class PauseScene : public Scene
{
public:
	// �R���X�g���N�^
	PauseScene(SceneManager& manager);

	// �f�X�g���N�^
	virtual ~PauseScene();

	// �X�V
	virtual void Update() override;

	// �`��
	void Draw();
private:
	// ����
	enum class Item
	{
		DEBUG_SCENE,
		NUM
	};
private:
	// ���ݑI�𒆂̍���
	int currentSelectItem_;
};

