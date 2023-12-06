#pragma once
#include "SceneBase.h"
#include "../Util/Timer.h"
#include "../StateMachine.h"
#include "../Vector2.h"
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
class ScreenEffect;

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
	void EntarNormalState();
	void EntarGameClearState();
	void EntarGameOverState();

	void UpdateNormalState();
	void UpdateGameClearState();
	void UpdateGameOverState();

	void ExitNormalState();
	void ExitGameClearState();
	void ExitGameOverState();

	// �����蔻��
	void Collision();

private:
	enum class SceneItem
	{
		PAUSE,
		RESULT, 
		TITLE
	};

	enum class State
	{
		NORMAL,
		GAME_CLEAR,
		GAME_OVER,
	};

private:
	// �V�[���A�C�e��
	SceneItem item_;

	// �X�e�[�g�}�V��
	StateMachine<State> stateMachine_;

	// �Q�[���J�n����̌o�ߎ���
	Timer<int> gameTimer_;

	// �{�X�̎��S���o�̃t���[��
	Timer<int> bossDiedEffectFrame_;

	Timer<int> waitTimer_;

	int screenHandle_;

	// �|�C���^
	std::shared_ptr<Background> pBackground_;
	std::shared_ptr<Camera> pCamera_;
	std::shared_ptr<Player> pPlayer_;
	std::shared_ptr<RockManager> pRockManager_;
	std::shared_ptr<DataReaderFromUnity> pDataReader_;
	std::shared_ptr<PlanetManager> pPlanetManager_;
	std::shared_ptr<EnemyManager> pEnemyManager_;
	std::shared_ptr<LaserManager> pLaserManager_;
	std::shared_ptr<ScreenEffect> pScreenEffect_;
};

