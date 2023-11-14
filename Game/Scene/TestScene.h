#pragma once
#include "SceneBase.h"
#include <memory>

// �v���g�^�C�v�錾
class RockManager;
class PlanetManager;
class Camera;
class Player;
class DataReaderFromUnity;
class EnemyManager;
class LazerManager;
class Background;

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
	virtual ~TestScene();

	// �����o�֐��|�C���^�̍X�V
	void Update();

	// �`��
	void Draw();
private:
	// �ʏ�̍X�V
	void NormalUpdate();

	// �Q�[���I�[�o�[���̍X�V
	void CollisionRockUpdate();

private:
	enum class SceneItem
	{
		PAUSE,
		RESULT, 
		TITLE
	};
private:
	int windEffectH_;
	int backGroundH_;
	SceneItem item_;

	// �����o�֐��|�C���^
	// Update��؂�ւ��邽�߂ɍ쐬
	void (TestScene::* updateFunc_) ();

	// �|�C���^
	std::shared_ptr<Background> pBackground_;
	std::shared_ptr<Camera> pCamera_;
	std::shared_ptr<Player> pPlayer_;
	std::shared_ptr<RockManager> pRockManager_;
	std::shared_ptr<DataReaderFromUnity> pDataReader_;
	std::shared_ptr<PlanetManager> pPlanetManager_;
	std::shared_ptr<EnemyManager> pEnemyManager_;
	std::shared_ptr<LazerManager> pLazerManager_;
};

