#pragma once
#include <list>
#include <unordered_map>
#include <vector>
#include "EnemyBase.h"

class LazerManager;
class Player;
class HpBar;

class EnemyManager
{
public:
	EnemyManager(std::shared_ptr<Player> pPlayer, std::shared_ptr<LazerManager> pLazerManager, UnityGameObject bossEnemyData, std::vector<UnityGameObject> normalEnemyData);
	virtual ~EnemyManager();

	void Update();
	void Draw();

	void OnDamage(float damage);

	// �S�Ă̓G����ʊO�܂œ��������m�F
	void CheckRunAllEnemy();

	// �Q�b�^�[
	bool GetIsRepel() const;	// ���ނ��ꂽ��
	const std::list<std::shared_ptr<EnemyBase>>& GetEnemies() const;	

private:
	// �G
	std::list<std::shared_ptr<EnemyBase>> pEnemies_;

	// ���f���n���h���e�[�u��
	std::unordered_map<EnemyType, int> modelHandleTable_;

	// HP
	std::unique_ptr<HpBar> pHpBar_;		// HP�o�[
	float hp_;							// HP

	// ���ނ��ꂽ��
	bool isRepel_;
};