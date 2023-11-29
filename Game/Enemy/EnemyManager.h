#pragma once
#include <list>
#include <string>
#include <unordered_map>
#include <vector>
#include "EnemyBase.h"

class LazerManager;
class Player;
class Warning;

struct EnemyAIData
{
	VECTOR goalPos;			// �ړI�n
	float speed;			// �ړI�n�Ɍ��������x
	float idleTime;			// �ړI�n�ɓ��B���Ă��玟�̖ړI�n�Ɍ������܂ł̑ҋ@�t���[��
	bool isLaser;			// �ړI�n�ɓ��B�����烌�[�U�[������
	int laserType;			// ���[�U�[�����ꍇ�A�ǂ̃��[�U�[������
	int laserFireFrameCount;// ���[�U�[�����t���[�����˂������邩
	float laserIdleTime;	// ���[�U�[�����ꍇ�A�ړI�n�ɓ��B���Ă��烌�[�U�[�����܂ł̑ҋ@�t���[��
};

class EnemyManager
{
public:
	EnemyManager(std::shared_ptr<Player> pPlayer, std::shared_ptr<LazerManager> pLazerManager);
	~EnemyManager();

	void Update(int time);
	void Draw();
	void DrawUI();

	const std::list<std::shared_ptr<EnemyBase>>& GetEnemies() const;	

private:
	void NormalUpdate();
	void CreateBossEnemyUpdate();

	void DeleteNotEnabledEnemy();
	void NormalEnemyEntry(const std::string filePath);

private:
	// �����o�֐��|�C���^
	// Update��؂�ւ��邽�߂ɍ쐬
	void (EnemyManager::*updateFunc_) ();

	std::list<std::shared_ptr<EnemyBase>> pEnemies_;
	std::shared_ptr<Player> pPlayer_;
	std::shared_ptr<LazerManager> pLazerManager_;
	std::unique_ptr<Warning> pWarning_;

	// ���f���n���h���e�[�u��
	std::unordered_map<EnemyType, int> modelHandleTable_;

	// �{�X�G�𐶐�������
	bool isCreateBossEnemy_;
};