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
	void Update(const InputState& input);

	// �`��
	void Draw();

private:
	// �����o�֐��|�C���^
	using UpdateFunc_t = void (MainScene::*) (const InputState& input);
	UpdateFunc_t updateFunc_;

	// �ʏ�̍X�V
	void NormalUpdate(const InputState& input);

private:
};