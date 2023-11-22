#pragma once
#include "EnemyBase.h"

class HpBar;

class BossEnemy : public EnemyBase
{
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="modelHandle"></param>
	/// <param name="pPlayer"></param>
	/// <param name="pLazerManager"></param>
	BossEnemy(int modelHandle, std::shared_ptr<Player> pPlayer, std::shared_ptr<LazerManager> pLazerManager);

	// �f�X�g���N�^
	virtual ~BossEnemy();

	// �X�V
	void Update() override;

	// �`��
	void Draw() override;		// ���f���A���̑�
	void DrawUI() override;		// UI

	/// <summary>
	/// �_���[�W
	/// </summary>
	/// <param name="damage">�󂯂��_���[�W</param>
	/// <param name="pos">�_���[�W���󂯂��ʒu</param>
	void OnDamage(int damage, VECTOR pos) override;

private:
	// �X�e�[�g�̏�����
	void InitState() override;

	// �ړ������̏�����
	void InitMove();

	void Move();
	void CubeLaserAttack();
	void NormalLaserAttack();

private:
	// Entar
	void EntarEntry();
	void EntarIdle();
	void EntarMove();
	void EntarDamage();
	void EntarDied();
	void EntarStopCubeLaserAttack();
	void EntarStopNormalLaserAttack();
	void EntarMoveCubeLaserAttack();
	void EntarMoveNormalLaserAttack();

	// Update
	void UpdateEntry();
	void UpdateIdle();
	void UpdateMove();
	void UpdateDamage();
	void UpdateDied();
	void UpdateStopCubeLaserAttack();
	void UpdateStopNormalLaserAttack();
	void UpdateMoveCubeLaserAttack();
	void UpdateMoveNormalLaserAttack();

	// Exit
	void ExitEntry();
	void ExitIdle();
	void ExitMove();
	void ExitDamage();
	void ExitDied();
	void ExitStopCubeLaserAttack();
	void ExitStopNormalLaserAttack();
	void ExitMoveCubeLaserAttack();
	void ExitMoveNormalLaserAttack();

private:
	enum class State
	{
		ENTRY,
		IDLE,
		MOVE,
		DAMAGE,
		DEID,

		STOP_ATTACK_CUBE_LASER,
		STOP_ATTACK_NORMAL_LASER,
		MOVE_ATTACK_CUBE_LASER,
		MOVE_ATTACK_NORMAL_LASER,
	};

private:
	// �X�e�[�g�}�V��(�X�e�[�g���Ǘ�����)
	StateMachine<State> stateMachine_;

	// HP�o�[
	std::unique_ptr<HpBar> pHpBar_;

	// �L���[�u���[�U�[�̈ړ����x
	float cubeLaserSpeed_;

	// �ʏ탌�[�U�[�̔��ˈʒu
	VECTOR normalLaserFirePos_;

	// �ړ�
	std::vector<VECTOR> movePointTable_;	// �ړ�����n�_�̃e�[�u��
	int movePoint_;		// ���݂̈ړ��n�_
	VECTOR goalPos_;	// �ڕW�̒n�_�̈ʒu
	bool isGoal_;		// �ڕW�n�_�ɓ��B������
	bool isMoveEnd_;	// �S�Ă̒n�_�ɓ��B���Ĉړ����I��������
};