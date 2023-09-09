#pragma once
#include "Scene.h"

class InputState;
class SceneManager;

/// <summary>
/// �L�[�R���t�B�O�V�[��
/// </summary>
class KeyConfigScene : public Scene
{
public:
	KeyConfigScene(SceneManager& manager, const InputState& input);
	virtual ~KeyConfigScene();

	void Update(const InputState& input);
	void Draw();

private:
	const InputState& inputState_;

	//���ݑI�𒆂̃C���f�b�N�X
	int currentInputIndex_ = 0;

	//���ݓ���̓��͕͂ҏW���ł�
	bool isEditing_ = false;
};

