#include "EnemyManager.h"
#include "BossEnemy.h"
#include "NormalEnemy.h"
#include "EnemyBase.h"
#include "../Player.h"
#include "../common.h"
#include "../Util/Debug.h"
#include "../Util/DrawFunctions.h"
#include "../Util/StringManager.h"
#include "../UI/Warning.h"
#include <string>
#include <DxLib.h>
#include <cassert>
#include <fstream>
#include <sstream>

namespace
{
	// �t�@�C���̊K�w
	const std::string model_file_hierarchy = "Data/Model/MV1/";

	// �t�@�C���̊g���q
	const std::string model_file_extension = ".mv1";

	// �t�@�C����
	const std::string normal_enemy_model_file_name = "NormalEnemy2";
	const std::string boss_enemy_model_file_name = "BossEnemy";

	// ���t���[���ԁA�{�X�o����WARNING!!��`�悷�邩
	constexpr int warning_ui_draw_frame = 60 * 4;

	// �Q�[���J�n���牽�t���[���o�߂�����{�X���o�������邩
	constexpr int boss_create_frame = 60 * 10;
}

EnemyManager::EnemyManager(std::shared_ptr<Player> pPlayer, std::shared_ptr<LazerManager> pLazerManager) :
	pPlayer_(pPlayer),
	pLazerManager_(pLazerManager),
	isCreateBossEnemy_(false),
	pWarning_(nullptr),
	updateFunc_(&EnemyManager::NormalUpdate)
{
	// ���f���̃��[�h
	std::string normalEnemyFilePath = model_file_hierarchy + normal_enemy_model_file_name + model_file_extension;
	modelHandleTable_[EnemyType::NOMAL] = my::MyLoadModel(normalEnemyFilePath.c_str());

	std::string bossEnemyFilePath = model_file_hierarchy + boss_enemy_model_file_name + model_file_extension;
	modelHandleTable_[EnemyType::BOSS] = my::MyLoadModel(bossEnemyFilePath.c_str());

	// �G���G�̃C���X�^���X�̍쐬
	/*pEnemies_.push_back(std::make_shared<NormalEnemy>(
		modelHandleTable_[EnemyType::NOMAL],
		pPlayer_,
		pLazerManager_));*/
	NormalEnemyEntry();
}

EnemyManager::~EnemyManager()
{
	for (auto& handle : modelHandleTable_)
	{
		MV1DeleteModel(handle.second);
	}
}

void EnemyManager::Update(int time)
{
	// ���݂��Ă��Ȃ��G�̍폜
	DeleteNotEnabledEnemy();

	if (boss_create_frame < time &&
		!isCreateBossEnemy_)
	{
		isCreateBossEnemy_ = true;
		pEnemies_.clear();

		pWarning_ = std::make_unique<Warning>(warning_ui_draw_frame);

		// update���{�X�o���update�ɐ؂�ւ�
		updateFunc_ = &EnemyManager::CreateBossEnemyUpdate;
	}

	Debug::Log("time", time / 60);

	(this->*updateFunc_)();
}	

void EnemyManager::Draw()
{
	for (auto& enemy : pEnemies_)
	{
		enemy->Draw();
	}
}

void EnemyManager::DrawUI()
{
	for (auto& enemy : pEnemies_)
	{
		enemy->DrawUI();
	}

	if (pWarning_)
	{
		pWarning_->Draw();
	}
}

const std::list<std::shared_ptr<EnemyBase>>& EnemyManager::GetEnemies() const
{
	return pEnemies_;
}

void EnemyManager::NormalUpdate()
{
	for (auto& enemy : pEnemies_)
	{
		enemy->Update();
	}
}

void EnemyManager::CreateBossEnemyUpdate()
{
	pWarning_->Update();

	if (pWarning_->IsEnd())
	{
		// �G�{�X�̃C���X�^���X����
		pEnemies_.push_back(
			std::make_shared<BossEnemy>(modelHandleTable_[EnemyType::BOSS],
				pPlayer_,
				pLazerManager_));

		updateFunc_ = &EnemyManager::NormalUpdate;
	}
}

void EnemyManager::DeleteNotEnabledEnemy()
{
	// �s�v�ɂȂ����G�̍폜
	pEnemies_.remove_if([](std::shared_ptr<EnemyBase> enemy) { return !enemy->IsEnabled(); });
}

void EnemyManager::NormalEnemyEntry()
{
	// TODO : �G�f�B�^�[����� or �O���t�@�C����(�p�^�[���𕡐����) or Maya��blender���g���ăt���[����z�u���Ă�����ڎw���I��

	//// �t�@�C�����̓ǂݍ���(�ǂݍ��݂Ɏ��s������~�߂�)
	//std::ifstream ifs("Data/Csv/Enemy.csv");
	//assert(ifs);

	//// csv�f�[�^��1�s���ǂݎ��
	//std::vector<EnemyAIData> dataTable;
	//int index = 0;
	//bool isFirst = false;
	//std::string line;
	//while (getline(ifs, line))
	//{
	//	// 1�s�ڂ͓ǂݍ��܂Ȃ�
	//	// 1�s�ڂɂ͍��ڂ������Ă��邽��
	//	if (!isFirst)
	//	{
	//		isFirst = true;
	//		continue;
	//	}

	//	// csv�f�[�^�P�s��','�ŕ����̕�����ɕϊ�
	//	std::vector<std::string> strvec = StringManager::GetInstance().SplitString(line, ',');

	//	// csv�f�[�^����擾�����f�[�^����t�H���g�n���h���̍쐬���Ċi�[
	//	EnemyAIData data{};
	//	
	//	// �����ʒu�̕ۑ�
	//	data.initPos.x = std::stof(strvec[index]);
	//	index++;
	//	data.initPos.y = std::stof(strvec[index]);
	//	index++;
	//	data.initPos.z = std::stof(strvec[index]);
	//	index++;

	//	auto itr = strvec.begin();
	//	std::advance(itr, index);
	//	VECTOR pos{};
	//	for (itr; itr != strvec.end(); itr++)
	//	{
	//		// �ړI�n�̕ۑ�
	//		pos.x = std::stof(strvec[index]);
	//		index++;
	//		pos.y = std::stof(strvec[index]);
	//		index++;
	//		pos.z = std::stof(strvec[index]);
	//		index++;
	//		data.a.goalPos = pos;

	//		// �X�s�[�h�̕ۑ�
	//		data.a.speed = std::stof(strvec[index]);
	//		index++;

	//		// �V���b�g���邩�̕ۑ�
	//		data.a.isShot = std::stoi(strvec[index]);
	//		index++;

	//		// �ҋ@���Ԃ̕ۑ�
	//		data.a.idleTime = std::stoi(strvec[index]);
	//		index++;

	//		std::advance(itr, index);
	//	}
	//	dataTable.push_back(data);
	//}

	std::vector<NormalEnemyAIData> table;

	// �����ʒu
	VECTOR initPos = { -2520, 540, 400 };

	// �G1�̍s��1
	NormalEnemyAIData data{};
	data.goalPos = { -700, 300, 800 };
	data.speed = 15.0f;
	data.idleTime = 180.0f;
	data.isShot = true;
	table.push_back(data);

	// �G1�̍s��2
	data.goalPos = initPos;
	data.speed = 15.0f;
	data.idleTime = 0.0f;
	data.isShot = false;
	table.push_back(data);

	// �G1�̃C���X�^���X�̍쐬
	pEnemies_.push_back(
		std::make_shared<NormalEnemy>(
		modelHandleTable_[EnemyType::NOMAL], 
		pPlayer_, 
		pLazerManager_,
		initPos,
		table));

	std::vector<NormalEnemyAIData> table2;
	initPos = { -2520, -540, 400 };

	// �G2�̍s��1
	data.goalPos = { -700, -300, 800 };
	data.speed = 15.0f;
	data.idleTime = 180.0f;
	data.isShot = true;
	table2.push_back(data);

	// �G2�̍s��2
	data.goalPos = initPos;
	data.speed = 15.0f;
	data.idleTime = 0.0f;
	data.isShot = false;
	table2.push_back(data);

	// �G2�̃C���X�^���X�̍쐬
	pEnemies_.push_back(
		std::make_shared<NormalEnemy>(
			modelHandleTable_[EnemyType::NOMAL],
			pPlayer_,
			pLazerManager_,
			initPos,
			table2));
}