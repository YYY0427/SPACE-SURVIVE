#pragma once
#include "Scene.h"

class InputState;

/// <summary>
/// �L�[�R���t�B�O�V�[��
/// </summary>
class KeyConfigScene : public Scene
{
public:
	// �R���X�g���N�^
	KeyConfigScene(SceneManager& manager, const InputState& input);

	// �f�X�g���N�^
	virtual ~KeyConfigScene();

	// �X�V
	void Update(const InputState& input);

	// �`��
	void Draw();

private:
	const InputState& inputState_;

	// ���ݑI�𒆂̃C���f�b�N�X
	int currentSelectIndex_;

	// ���ݓ���̓��͕͂ҏW�����ǂ���
	bool isEditing_;
};

