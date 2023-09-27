#pragma once
#include "Scene.h"
#include <array>

// �f�o�b�O�p�V�[��
// �f�o�b�O�����₷�����邽�߂ɍ쐬
// �e�V�[���ɔ�ׂ�
class DebugScene : public Scene
{
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="manager">�V�[���}�l�[�V���[�̎Q��</param>
	DebugScene(SceneManager& manager);

	// �f�X�g���N�^
	virtual ~DebugScene();

	// �X�V
	void Update();

	// �`��
	void Draw();
private:
	// �f�o�b�O�V�[�������ׂ�V�[���̍���
	enum class SceneItem
	{
		TEST_SCENE,			// �e�X�g�V�[��
		TITLE_SCENE,		// �^�C�g���V�[��
		MAIN_SCENE,			// ���C���V�[��
		CONFIG_SCENE,		// �ݒ�V�[��
		PAUSE_SCENE,		// �|�[�Y�V�[��
		TOTAL_VALUE			// ���ڂ̍��v�l
	};
private:
	// ���ݑI�𒆂̍���
	int currentSelectItem_;
};