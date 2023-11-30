#pragma once
#include <list>
#include <string>
#include <unordered_map>
#include <vector>
#include "EnemyBase.h"

class LaserManager;
class Player;
class Warning;

struct NormalEnemyActionData
{
	VECTOR goalPos;			// �ړI�n
	float speed;			// �ړI�n�Ɍ��������x
	int idleTime;			// �ړI�n�ɓ��B���Ă��玟�̖ړI�n�Ɍ������܂ł̑ҋ@�t���[��
	bool isLaser;			// �ړI�n�ɓ��B�����烌�[�U�[������
	int laserType;			// ���[�U�[�����ꍇ�A�ǂ̃��[�U�[������
	int laserIdleFrame;		// ���[�U�[�����ꍇ�A�ړI�n�ɓ��B���Ă��烌�[�U�[�����܂ł̑ҋ@�t���[��
	int laserFireFrameTime; // ���[�U�[�����t���[�����˂������邩
	float cubeLaserSpeed;	// �L���[�u���[�U�[�̑��x
	int laserChargeFrame;	// ���[�U�[�̃`���[�W�t���[��
};

class EnemyManager
{
public:
	EnemyManager(std::shared_ptr<Player> pPlayer, std::shared_ptr<LaserManager> pLaserManager);
	~EnemyManager();

	void Update(int time);
	void Draw();
	void DrawUI();

	const std::list<std::shared_ptr<EnemyBase>>& GetEnemies() const;	

private:
	void NormalUpdate();
	void CreateBossEnemyUpdate();

	/// <summary>
	/// �G���G�̍s���t�@�C���f�[�^�̃��[�h�ƕۑ�
	/// </summary>
	/// <param name="filePath">�t�@�C���p�X</param>
	void LoadAndStoreNormalEnemyActionFileData(const std::string filePath);

private:
	// �����o�֐��|�C���^
	// Update��؂�ւ��邽�߂ɍ쐬
	void (EnemyManager::*updateFunc_) ();

	std::list<std::shared_ptr<EnemyBase>> pEnemies_;
	std::shared_ptr<Player> pPlayer_;
	std::shared_ptr<LaserManager> pLaserManager_;
	std::unique_ptr<Warning> pWarning_;

	// ���f���n���h���e�[�u��
	std::unordered_map<EnemyType, int> modelHandleTable_;

	// �{�X�G�𐶐�������
	bool isCreateBossEnemy_;
};