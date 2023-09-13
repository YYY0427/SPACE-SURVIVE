#pragma once
#include "Scene.h"

using namespace std;

/// <summary>
/// �f�o�b�O�p�V�[��
/// </summary>
class DebugScene : public Scene
{
public:
	//�R���X�g���N�^
	DebugScene(SceneManager& manager);

	// �f�X�g���N�^
	virtual ~DebugScene();

	// �X�V
	void Update(const InputState& input);

	// �`��
	void Draw();
private:
	// �f�o�b�O�V�[�������ׂ�V�[���B
	enum SceneName
	{
		TEST_SCENE,
		TITLE_SCENE,
		MAIN_SCENE,
		NUM
	};
private:
	// ���ݑI�𒆂̃C���f�b�N�X
	int currentSelectIndex_;
};