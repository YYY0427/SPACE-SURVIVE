#pragma once
#include "Scene.h"

class InputState;

/// <summary>
/// �^�C�g���V�[��
/// </summary>
class TitleScene : public Scene
{
public:
	TitleScene(SceneManager& manager);
	~TitleScene();

	void Update(const InputState& input);
	void Draw();

private:
	int fadeTimer_;			// �t�F�[�h�^�C�}�[
	int fadeValue_ = 255;	// ����`�Ƃ̃u�����h�

	// �t�F�[�h�C���̎���Update�֐�
	void FadeInUpdate(const InputState& input);

	// �ʏ��Ԃ�Update�֐�
	void NormalUpdate(const InputState& input);

	// �t�F�[�h�A�E�g�̎���Update�֐�
	void FadeOutUpdate(const InputState& input);

	// Update�p�����o�֐��|�C���^
	void (TitleScene::* updateFunc_)(const InputState& input);
};