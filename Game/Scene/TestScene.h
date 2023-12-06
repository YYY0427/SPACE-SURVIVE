#pragma once
#include "SceneBase.h"
#include <memory>
#include "../Vector2.h"

// �v���g�^�C�v�錾
class Camera;
class Warning;
class HpBar;

// �e�X�g�V�[��
// �F�X�����悤�̃V�[��
class TestScene : public SceneBase
{
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="manager">�V�[���}�l�[�W���[�̎Q��</param>
	TestScene(SceneManager& manager);

	// �f�X�g���N�^
	~TestScene();

	// �����o�֐��|�C���^�̍X�V
	void Update() override;
	
	// �`��
	void Draw() override;

private:
	// �|�C���^
	std::shared_ptr<Camera> pCamera_;
	std::shared_ptr<Warning> pWarning_;
	std::shared_ptr<HpBar> pHpBar_;
	int laserEffectHandle_;
	int imgHandle_;
	Vector2 imgPos_;
};