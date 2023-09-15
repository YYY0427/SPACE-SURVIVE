#pragma once
#include "Scene.h"

/// <summary>
/// ���C���V�[��
/// </summary>
class MainScene : public Scene
{
public:
	// �R���X�g���N�^
	MainScene(SceneManager& manager);

	// �f�X�g���N�^
	virtual ~MainScene();

	// �X�V
	void Update();

	// �`��
	void Draw();

private:
	// �����o�֐��|�C���^
	using UpdateFunc_t = void (MainScene::*) ();
	UpdateFunc_t updateFunc_;

	// �ʏ�̍X�V
	void NormalUpdate();

private:
};