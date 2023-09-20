#pragma once
#include "Scene.h"

// �|�[�Y�V�[��
class PauseScene : public Scene
{
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="manager">�V�[���}�l�[�W���[�̎Q��</param>
	PauseScene(SceneManager& manager);

	// �f�X�g���N�^
	virtual ~PauseScene();

	// �X�V
	virtual void Update() override;

	// �`��
	void Draw();
private:
	// �|�[�Y�V�[������I���ł��鍀��
	enum class Item
	{
		DEBUG_SCENE,	// �f�o�b�O�V�[��
		TOTAL_VALUE		// ���ڂ̍��v�l
	};
private:
	// ���ݑI�𒆂̍���
	int currentSelectItem_;
};

