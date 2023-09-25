#pragma once
#include "Scene.h"

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
	enum class Item
	{
		GAME_START,
		OPSITON,
		GAME_END,				
		TOTAL_VALUE			// ���ڂ̍��v�l
	};
private:
	// �����o�֐��|�C���^
	// Update��؂�ւ��邽�߂ɍ쐬
	void (TitleScene::* updateFunc_)();
};