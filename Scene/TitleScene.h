#pragma once
#include "Scene.h"

/// <summary>
/// �^�C�g���V�[��
/// </summary>
class TitleScene : public Scene
{
public:
	// �R���X�g���N�^
	TitleScene(SceneManager& manager);

	// �f�X�g���N�^
	virtual ~TitleScene();

	// �X�V
	void Update();

	// �`��
	void Draw();
private:
	// �ʏ��Ԃ̍X�V
	void NormalUpdate();

	// �����o�֐��|�C���^
	void (TitleScene::* updateFunc_)();
};