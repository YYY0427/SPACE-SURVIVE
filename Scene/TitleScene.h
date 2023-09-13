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
	// �ʏ��Ԃ�Update�֐�
	void NormalUpdate(const InputState& input);

	// Update�p�����o�֐��|�C���^
	void (TitleScene::* updateFunc_)(const InputState& input);
};