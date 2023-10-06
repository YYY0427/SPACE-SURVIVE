#pragma once
#include "Scene.h"
#include <memory>

// �v���g�^�C�v�錾
class RockManager;
class PlanetManager;
class SkyDome;
class Camera;
class Player;
class DataReaderFromUnity;
class Image3DManager;

// �e�X�g�V�[��
// �F�X�����悤�̃V�[��
class TestScene : public Scene
{
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="manager">�V�[���}�l�[�W���[�̎Q��</param>
	TestScene(SceneManager& manager);

	// �f�X�g���N�^
	virtual ~TestScene();

	// �����o�֐��|�C���^�̍X�V
	void Update();

	// �`��
	void Draw();
private:
	// �ʏ�̍X�V
	void NormalUpdate();

	// �Q�[���I�[�o�[���̍X�V
	void GameOverUpdate();

	// �n�ʂ̐��̕`��
	void GroundLineDraw();
private:
	// �����o�֐��|�C���^
	// Update��؂�ւ��邽�߂ɍ쐬
	void (TestScene::* updateFunc_) ();

	// �|�C���^
	std::shared_ptr<Camera> pCamera_;
	std::shared_ptr<Player> pPlayer_;
	std::shared_ptr<RockManager> pRockManager_;
	std::shared_ptr<SkyDome> pSkyDome_;
	std::shared_ptr<DataReaderFromUnity> pDataReader_;
	std::shared_ptr<PlanetManager> pPlanetManager_;
	std::shared_ptr<Image3DManager> pImg3DManager_;
};

