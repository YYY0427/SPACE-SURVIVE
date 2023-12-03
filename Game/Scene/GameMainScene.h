#pragma once
#include "SceneBase.h"
#include "../Util/Timer.h"
#include <memory>

// �v���g�^�C�v�錾
class RockManager;
class PlanetManager;
class Camera;
class Player;
class DataReaderFromUnity;
class EnemyManager;
class LaserManager;
class Background;

// ���C���V�[��
// �Q�[���̃��C���̏������s���V�[��
class GameMainScene : public SceneBase
{
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="manager">�V�[���}�l�[�W���[�̎Q��</param>
	GameMainScene(SceneManager& manager);

	// �f�X�g���N�^
	~GameMainScene();

	// �����o�֐��|�C���^�̍X�V
	void Update() override;

	// �`��
	void Draw() override;

private:
	// �ʏ�̍X�V
	void NormalUpdate();

private:
	enum class SceneItem
	{
		PAUSE,
		RESULT, 
		TITLE
	};

private:
	// �V�[���A�C�e��
	SceneItem item_;

	// �Q�[���N���A�t���O
	bool isGameClear_;

	// �Q�[���J�n����̌o�ߎ���
	Timer<int> gameTimer_;

	// �Q�[���N���A���ɉ��t���[���ԉ�ʂ�h�炷��
	Timer<int> quakeTimer_;
	
	// �{�X�̎��S���o�̃t���[��
	Timer<int> bossDiedEffectFrame_;

	float quakeX_;

	int screenHandle_;

	// �����o�֐��|�C���^
	// Update��؂�ւ��邽�߂ɍ쐬
	void (GameMainScene::* updateFunc_) ();

	// �|�C���^
	std::shared_ptr<Background> pBackground_;
	std::shared_ptr<Camera> pCamera_;
	std::shared_ptr<Player> pPlayer_;
	std::shared_ptr<RockManager> pRockManager_;
	std::shared_ptr<DataReaderFromUnity> pDataReader_;
	std::shared_ptr<PlanetManager> pPlanetManager_;
	std::shared_ptr<EnemyManager> pEnemyManager_;
	std::shared_ptr<LaserManager> pLaserManager_;
};

