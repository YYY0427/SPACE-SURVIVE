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
	// 雑魚敵の行動データのファイルパス
	const std::string normal_eneny_action_file_path_test = "Data/Csv/NormalEnemy/ActionTest.csv";
	const std::string normal_eneny_action_file_path_1 = "Data/Csv/NormalEnemy/Action1.csv";

	// モデルのファイルパス
	const std::string boss_enemy_model_file_path_1 = "Data/Model/BossEnemy1.mv1";
	const std::string normal_enemy_model_file_path_1 = "Data/Model/NormalEnemy1.mv1";

	// 何フレーム間、ボス出現のWARNING!!を描画するか
	constexpr int warning_ui_draw_frame = 60 * 1;

	// ゲーム開始から何フレーム経過したらボスを出現させるか
	constexpr int boss_create_frame = 60 * 1;
}

EnemyManager::EnemyManager(std::shared_ptr<Player> pPlayer, std::shared_ptr<LaserManager> pLaserManager, std::shared_ptr<ScreenEffect> pScreenEffect, std::shared_ptr<UIManager> pUIManager) :
	pPlayer_(pPlayer),
	pLaserManager_(pLaserManager),
	pScreenEffect_(pScreenEffect),
	pUIManager_(pUIManager),
	isCreateBossEnemy_(false),
	updateFunc_(&EnemyManager::NormalUpdate)
{
	// モデルのロード
	modelHandleTable_[EnemyType::NOMAL] = my::MyLoadModel(normal_enemy_model_file_path_1.c_str());
	modelHandleTable_[EnemyType::BOSS] = my::MyLoadModel(boss_enemy_model_file_path_1.c_str());

	// 雑魚敵のインスタンスの作成
	LoadAndStoreNormalEnemyActionFileData(normal_eneny_action_file_path_1);
}

EnemyManager::~EnemyManager()
{
	// 全てのモデルの解放
	for (auto& handle : modelHandleTable_)
	{
		MV1DeleteModel(handle.second);
	}
}

void EnemyManager::Update(int time)
{
	// 不要になった敵の削除
	pEnemies_.remove_if([](std::shared_ptr<EnemyBase> enemy) { return !enemy->IsEnabled(); });

	// ボス出現していない、ボス出現の時間に到達したらボス出現
	if (boss_create_frame < time &&
		!isCreateBossEnemy_)
	{
		// ボス出現フラグを立てる
		isCreateBossEnemy_ = true;

		// 全ての敵の削除
		pEnemies_.clear();

		// ボス出現のUI演出を開始
		pWarning_ = std::make_unique<Warning>(warning_ui_draw_frame);

		// updateをボス登場のupdateに切り替え
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

// 敵の取得
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
	// 敵ボス出現のUIの更新
	pWarning_->Update();

	// 敵ボス出現のUI演出が終了したか
	if (pWarning_->IsEnd())
	{
		pWarning_.reset();

		// 敵ボスのインスタンス生成
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
	// ファイル情報の読み込み(読み込みに失敗したら止める)
	std::ifstream ifs(filePath);
	assert(ifs);

	// csvデータを1行ずつ読み取る
	bool isFirst = false;
	std::string line;
	while (getline(ifs, line))
	{
		// 1行目は読み込まない
		// 1行目には項目が書いてあるため
		if (!isFirst)
		{
			isFirst = true;
			continue;
		}

		// 初期化
		std::vector<NormalEnemyActionData> dataTable{};
		int index = 0;

		// csvデータ１行を','で複数の文字列に変換
		std::vector<std::string> strvec = StringManager::GetInstance().SplitString(line, ',');
		
		// 初期位置の保存
		VECTOR initPos{};
		initPos.x = std::stof(strvec[index]);
		index++;
		initPos.y = std::stof(strvec[index]);
		index++;
		initPos.z = std::stof(strvec[index]);
		index++;

		// 最終目的地に到達したときに消すかの保存
		bool isErase = std::stoi(strvec[index]);
		index++;

		// 位置は複数地点設定できるため繰り返す
		while(index < strvec.size())
		{
			NormalEnemyActionData data{};

			// 目的地の保存
			data.goalPos.x = std::stof(strvec[index]);
			index++;
			data.goalPos.y = std::stof(strvec[index]);
			index++;
			data.goalPos.z = std::stof(strvec[index]);
			index++;

			// スピードの保存
			data.speed = std::stof(strvec[index]);
			index++;

			// 待機時間の保存
			data.idleTime = std::stoi(strvec[index]);
			index++;

			// レーザーを撃つのかの保存
			data.isLaser = std::stoi(strvec[index]);
			index++;

			// レーザータイプの保存
			data.laserType = std::stoi(strvec[index]);
			index++;

			// レーザーを撃つまでの時間の保存
			data.laserIdleFrame = std::stoi(strvec[index]);
			index++;

			// レーザーを何フレーム発射し続けるかの保存 
			data.laserFireFrameTime = std::stoi(strvec[index]);
			index++;

			// キューブレーザーの速度の保存
			data.cubeLaserSpeed = std::stof(strvec[index]);
			index++;

			// レーザーのチャージフレームの保存
			data.laserChargeFrame = std::stoi(strvec[index]);
			index++;

			dataTable.push_back(data);
		}

		// 読み込んだ情報をもとに敵を作成
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