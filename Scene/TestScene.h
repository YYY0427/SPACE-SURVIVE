#pragma once
#include "Scene.h"
#include <memory>

class EnemyManager;
class SkyDome;
class Camera;
class Player;

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
	// �����o�֐��|�C���^
	using UpdateFunc_t = void (TestScene::*) ();
	UpdateFunc_t updateFunc_;

	// �ʏ�̍X�V
	void NormalUpdate();

	// 
	void GameOverUpdate();

	std::shared_ptr<Camera> pCamera_;
	std::shared_ptr<Player> pPlayer_;
	std::shared_ptr<EnemyManager> pEnemyManager_;
	std::shared_ptr<SkyDome> pSkyDome_;
};

