#pragma once
#include "Scene.h"

// �ݒ�V�[��
class ConfigScene : public Scene
{
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="manager">�V�[���}�l�[�W���[�̎Q��</param>
	ConfigScene(SceneManager& manager);

	// �f�X�g���N�^
	virtual ~ConfigScene();

	// �X�V
	void Update();

	// �`��
	void Draw();
private:
	// �ݒ�V�[���̍���
	enum class Item
	{
		BGM,	
		SE,
		TOTAL_VALUE			// ���ڂ̍��v�l
	};
private:
	// ���ݑI�𒆂̍���
	int currentSelectItem_;
};