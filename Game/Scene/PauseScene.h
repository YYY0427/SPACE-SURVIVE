#pragma once
#include "Scene.h"
#include <array>

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
	// �t�F�[�h�ݒ�f�[�^
	struct FadeData
	{
		int fadeValue;
		int fadeSpeed;
		bool isFade;
		bool isGaussFade;
		bool isOnlyStringFade;
	};
private:
	// ����
	enum class Item
	{
		CONTINUE,		// ������
		OPTION,			// �ݒ�V�[��
		TITLE,			// �^�C�g���V�[��
		TOTAL_VALUE		// ���ڂ̍��v�l
	};
private:
	// �t�F�[�h�̐ݒ�f�[�^�e�[�u��
	std::array<FadeData, static_cast<int>(Item::TOTAL_VALUE)> fadeConfigTable_;

	// �I�𒆂̍���
	int currentSelectItem_;
};
