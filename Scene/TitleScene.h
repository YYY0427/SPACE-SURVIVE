#pragma once
#include "Scene.h"

class InputState;

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
	void Update(const InputState& input);

	// �`��
	void Draw();
private:
	// �ʏ��Ԃ̍X�V
	void NormalUpdate(const InputState& input);

	// �����o�֐��|�C���^
	void (TitleScene::* updateFunc_)(const InputState& input);
};