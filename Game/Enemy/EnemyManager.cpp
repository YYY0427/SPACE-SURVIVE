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
#include <DxLib.h>
#include <cassert>
#include <fstream>
#include <sstream>

namespace
{
	// �G���G�̍s���f�[�^�̃t�@�C���p�X
	const std::string normal_eneny_action_file_path_test = "Data/Csv/NormalEnemy/ActionTest.csv";
	const std::string normal_eneny_action_file_path_1 = "Data/Csv/NormalEnemy/Action1.csv";

	// ���f���̃t�@�C���p�X
	const std::string boss_enemy_model_file_path_1 = "Data/Model/BossEnemy1.mv1";
	const std::string normal_enemy_model_file_path_1 = "Data/Model/NormalEnemy1.mv1";

	// ���t���[���ԁA�{�X�o����WARNING!!��`�悷�邩
	constexpr int warning_ui_draw_frame = 60 * 1;

	// �Q�[���J�n���牽�t���[���o�߂�����{�X���o�������邩
	constexpr int boss_create_frame = 60 * 100;
}

EnemyManager::EnemyManager(std::shared_ptr<Player> pPlayer, std::shared_ptr<LaserManager> pLaserManager, std::shared_ptr<ScreenEffect> pScreenEffect, std::shared_ptr<UIManager> pUIManager) :
	pPlayer_(pPlayer),
	pLaserManager_(pLaserManager),
	pScreenEffect_(pScreenEffect),
	pUIManager_(pUIManager),
	isCreateBossEnemy_(false),
	updateFunc_(&EnemyManager::NormalUpdate)
{
	// ���f���̃��[�h
	modelHandleTable_[EnemyType::NOMAL] = my::MyLoadModel(normal_enemy_model_file_path_1.c_str());
	modelHandleTable_[EnemyType::BOSS] = my::MyLoadModel(boss_enemy_model_file_path_1.c_str());

	// �G���G�̃C���X�^���X�̍쐬
	LoadAndStoreNormalEnemyActionFileData(normal_eneny_action_file_path_1);
}

EnemyManager::~EnemyManager()
{
	// �S�Ẵ��f���̉��
	for (auto& handle : modelHandleTable_)
	{
		MV1DeleteModel(handle.second);
	}
}

void EnemyManager::Update(int time)
{
	// �s�v�ɂȂ����G�̍폜
	pEnemies_.remove_if([](std::shared_ptr<EnemyBase> enemy) { return !enemy->IsEnabled(); });

	// �{�X�o�����Ă��Ȃ��A�{�X�o���̎��Ԃɓ��B������{�X�o��
	if (boss_create_frame < time &&
		!isCreateBossEnemy_)
	{
		// �{�X�o���t���O�𗧂Ă�
		isCreateBossEnemy_ = true;

		// �S�Ă̓G�̍폜
		pEnemies_.clear();

		// �{�X�o����UI���o���J�n
		pWarning_ = std::make_unique<Warning>(warning_ui_draw_frame);

		// update���{�X�o���update�ɐ؂�ւ�
		updateFunc_ = &EnemyManager::CreateBossEnemyUpdate;
	}

	(this->*updateFunc_)();
}

void EnemyManager::GameOverUpdate()
{
	for (auto& enemy : pEnemies_)
	{
		enemy->GameOverUpdate();
	}
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

bool EnemyManager::IsBossDied()
{
	if(!isCreateBossEnemy_)	return false;
	if(updateFunc_ == &EnemyManager::CreateBossEnemyUpdate)	return false;
	if(!pEnemies_.empty())	return false;	

	return true;
}

void EnemyManager::DeleteAllEnemy()
{
	pEnemies_.clear();
}

// �G�̎擾
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
	// �G�{�X�o����UI�̍X�V
	pWarning_->Update();

	// �G�{�X�o����UI���o���I��������
	if (pWarning_->IsEnd())
	{
		pWarning_.reset();

		// �G�{�X�̃C���X�^���X����
		pEnemies_.push_back(
			std::make_shared<BossEnemy>(modelHandleTable_[EnemyType::BOSS],
				pPlayer_,
				pLaserManager_,
				pScreenEffect_,
				pUIManager_));

		updateFunc_ = &EnemyManager::NormalUpdate;
	}
}

void EnemyManager::LoadAndStoreNormalEnemyActionFileData(const std::string filePath)
{
	// �t�@�C�����̓ǂݍ���(�ǂݍ��݂Ɏ��s������~�߂�)
	std::ifstream ifs(filePath);
	assert(ifs);

	// csv�f�[�^��1�s���ǂݎ��
	bool isFirst = false;
	std::string line;
	while (getline(ifs, line))
	{
		// 1�s�ڂ͓ǂݍ��܂Ȃ�
		// 1�s�ڂɂ͍��ڂ������Ă��邽��
		if (!isFirst)
		{
			isFirst = true;
			continue;
		}

		// ������
		std::vector<NormalEnemyActionData> dataTable{};
		int index = 0;

		// csv�f�[�^�P�s��','�ŕ����̕�����ɕϊ�
		std::vector<std::string> strvec = StringManager::GetInstance().SplitString(line, ',');
		
		// �����ʒu�̕ۑ�
		VECTOR initPos{};
		initPos.x = std::stof(strvec[index]);
		index++;
		initPos.y = std::stof(strvec[index]);
		index++;
		initPos.z = std::stof(strvec[index]);
		index++;

		// �ŏI�ړI�n�ɓ��B�����Ƃ��ɏ������̕ۑ�
		bool isErase = std::stoi(strvec[index]);
		index++;

		// �ʒu�͕����n�_�ݒ�ł��邽�ߌJ��Ԃ�
		while(index < strvec.size())
		{
			NormalEnemyActionData data{};

			// �ړI�n�̕ۑ�
			data.goalPos.x = std::stof(strvec[index]);
			index++;
			data.goalPos.y = std::stof(strvec[index]);
			index++;
			data.goalPos.z = std::stof(strvec[index]);
			index++;

			// �X�s�[�h�̕ۑ�
			data.speed = std::stof(strvec[index]);
			index++;

			// �ҋ@���Ԃ̕ۑ�
			data.idleTime = std::stoi(strvec[index]);
			index++;

			// ���[�U�[�����̂��̕ۑ�
			data.isLaser = std::stoi(strvec[index]);
			index++;

			// ���[�U�[�^�C�v�̕ۑ�
			data.laserType = std::stoi(strvec[index]);
			index++;

			// ���[�U�[�����܂ł̎��Ԃ̕ۑ�
			data.laserIdleFrame = std::stoi(strvec[index]);
			index++;

			// ���[�U�[�����t���[�����˂������邩�̕ۑ� 
			data.laserFireFrameTime = std::stoi(strvec[index]);
			index++;

			// �L���[�u���[�U�[�̑��x�̕ۑ�
			data.cubeLaserSpeed = std::stof(strvec[index]);
			index++;

			// ���[�U�[�̃`���[�W�t���[���̕ۑ�
			data.laserChargeFrame = std::stoi(strvec[index]);
			index++;

			dataTable.push_back(data);
		}

		// �ǂݍ��񂾏������ƂɓG���쐬
		pEnemies_.push_back(
		std::make_shared<NormalEnemy>(
		modelHandleTable_[EnemyType::NOMAL], 
		pPlayer_, 
		pLaserManager_,
		initPos,
		isErase,
		dataTable));
	}
}