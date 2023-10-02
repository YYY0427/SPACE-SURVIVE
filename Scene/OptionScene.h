#pragma once
#include "Scene.h"
#include <vector>

// �ݒ�V�[��
class OptionScene : public Scene
{
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="manager">�V�[���}�l�[�W���[�̎Q��</param>
	OptionScene(SceneManager& manager);

	// �f�X�g���N�^
	virtual ~OptionScene();

	// ������
	void Init();

	// �I������
	void End();

	// �X�V
	void Update();

	// �`��
	void Draw();
private:
	// ����
	enum class Item
	{
		LANGUAGE,			// ����
		WINDOW_MODE,		// �E�B���h�E���[�h
		MASTER_VOLUME,		// �S�̉���
		BGM_VOLUME,			// BGM����
		SE_VOLUME,			// SE����
		PAD_SETTING,		// �p�b�h�ݒ�V�[��
		BACK,				// �߂�
		TOTAL_VALUE			// ���ڂ̍��v�l
	};

private:
	// ���ݑI�𒆂̍���
	int currentSelectItem_;

	// ���ڂ̕`��F�f�[�^
	std::vector<int> itemColorTable_;

	// ���ʃA�C�R���̉摜�n���h��
	int soundIconImgHandle_;
};