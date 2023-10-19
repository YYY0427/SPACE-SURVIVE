#pragma once
#include "SceneBase.h"
#include <memory>

// �v���g�^�C�v�錾
class RockManager;
class SkyDome;
class Camera;
class Player;
class DataReaderFromUnity;

// ���C���V�[��
// �Q�[���̃��C���̏������s���V�[��
class MainScene : public SceneBase
{
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="manager">�V�[���}�l�[�W���[�̎Q��</param>
	MainScene(SceneManager& manager);

	// �f�X�g���N�^
	virtual ~MainScene();

	// �����o�֐��|�C���^�̍X�V
	void Update();
	
	// �`��
	void Draw();

private:
	// �ʏ�̍X�V
	void NormalUpdate();

	// �Q�[���I�[�o�[���̍X�V
	void GameOverUpdate();
private:
	// �����o�֐��|�C���^
	// Update��؂�ւ��邽�߂ɍ쐬
	void (MainScene::*updateFunc_) () ;

	// �|�C���^
	std::shared_ptr<Camera> pCamera_;
	std::shared_ptr<Player> pPlayer_;
	std::shared_ptr<RockManager> pRockManager_;
	std::shared_ptr<SkyDome> pSkyDome_;
	std::shared_ptr<DataReaderFromUnity> pDataReader_;
};