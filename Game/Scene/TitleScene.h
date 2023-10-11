#pragma once
#include "Scene.h"
#include <array>

// �^�C�g���V�[��
class TitleScene : public Scene
{
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="manager">�V�[���}�l�[�W���[�̎Q��</param>
	TitleScene(SceneManager& manager);

	// �f�X�g���N�^
	virtual ~TitleScene();

	// �����o�֐��|�C���^�̍X�V
	void Update();

	// �`��
	void Draw();

private:
	// �ʏ��Ԃ̍X�V
	void NormalUpdate();
private:
	// �t�F�[�h�ݒ�f�[�^
	struct FadeData
	{
		int fadeValue;
		int fadeSpeed;
		bool isFade;
		bool isGaussFade;
	};
private:
	// ����
	enum class Item
	{
		START,				// �Q�[���X�^�[�g
		OPSITON,			// �I�v�V����
		END,				// �Q�[���I��
		TOTAL_VALUE			// ���ڂ̍��v�l
	};
private:
	// �����o�֐��|�C���^
	// Update��؂�ւ��邽�߂ɍ쐬
	void (TitleScene::* updateFunc_)();

	// �摜�n���h��
	int handle_;

	// �t�F�[�h�ݒ�̃f�[�^�e�[�u��
	std::array<FadeData, static_cast<int>(Item::TOTAL_VALUE)> fadeDataTable_;

	// ���ݑI�𒆂̍���
	int currentSelectItem_;
};