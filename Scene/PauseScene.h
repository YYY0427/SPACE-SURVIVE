#pragma once
#include "Scene.h"

class InputState;

/// <summary>
/// �|�[�Y�V�[��
/// </summary>
class PauseScene : public Scene
{
public:
	// �R���X�g���N�^
	PauseScene(SceneManager& manager);

	// �f�X�g���N�^
	virtual ~PauseScene();

	// �X�V
	virtual void Update(const InputState& input) override;

	// �`��
	void Draw();
private:
};

