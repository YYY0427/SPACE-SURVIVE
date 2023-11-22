#pragma once
#include <list>
#include <unordered_map>
#include <vector>
#include "EnemyBase.h"

class LazerManager;
class Player;
class Warning;

class EnemyManager
{
public:
	EnemyManager(std::shared_ptr<Player> pPlayer, std::shared_ptr<LazerManager> pLazerManager);
	virtual ~EnemyManager();

	void Update(int time);
	void Draw();
	void DrawUI();

	const std::list<std::shared_ptr<EnemyBase>>& GetEnemies() const;	

private:
	void NormalUpdate();
	void CreateBossEnemyUpdate();
	void DeleteNotEnabledEnemy();

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