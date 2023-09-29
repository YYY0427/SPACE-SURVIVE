#pragma once
#include "Scene.h"

class Player;

// ���C���V�[��
// �Q�[���̃��C���̏������s���V�[��
class MainScene : public Scene
{
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="manager">�V�[���}�l�[�W���[�̎Q��</param>
	MainScene(SceneManager& manager);

	// �f�X�g���N�^
	virtual ~MainScene();

	// �����o�֐��|�C���^�̍X�V
	void Update();
	
	// �`��
	void Draw();

private:
	// �ʏ�̍X�V
	void NormalUpdate();

private:
	// �����o�֐��|�C���^
	// Update��؂�ւ��邽�߂ɍ쐬
	void (MainScene::*updateFunc_) () ;

	int handle_;
};