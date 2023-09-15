#pragma once
#include "Scene.h"
#include <memory>

using namespace std;

class Camera;

/// <summary>
/// �e�X�g�V�[��(�F�X�����悤�̃V�[��)
/// </summary>
class TestScene : public Scene
{
public:
	// �R���X�g���N�^
	TestScene(SceneManager& manager);

	// �f�X�g���N�^
	virtual ~TestScene();

	// �X�V
	void Update();

	// �`��
	void Draw();
private:
	shared_ptr<Camera> pCamera_;
};

