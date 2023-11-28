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
	// ファイルの階層
	const std::string model_file_hierarchy = "Data/Model/MV1/";

	// ファイルの拡張子
	const std::string model_file_extension = ".mv1";

	// ファイル名
	const std::string normal_enemy_model_file_name = "NormalEnemy2";
	const std::string boss_enemy_model_file_name = "BossEnemy";

	// 何フレーム間、ボス出現のWARNING!!を描画するか
	constexpr int warning_ui_draw_frame = 60 * 4;

	// ゲーム開始から何フレーム経過したらボスを出現させるか
	constexpr int boss_create_frame = 60 * 10;
}

EnemyManager::EnemyManager(std::shared_ptr<Player> pPlayer, std::shared_ptr<LazerManager> pLazerManager) :
	pPlayer_(pPlayer),
	pLazerManager_(pLazerManager),
	isCreateBossEnemy_(false),
	pWarning_(nullptr),
	updateFunc_(&EnemyManager::NormalUpdate)
{
	// モデルのロード
	std::string normalEnemyFilePath = model_file_hierarchy + normal_enemy_model_file_name + model_file_extension;
	modelHandleTable_[EnemyType::NOMAL] = my::MyLoadModel(normalEnemyFilePath.c_str());

	std::string bossEnemyFilePath = model_file_hierarchy + boss_enemy_model_file_name + model_file_extension;
	modelHandleTable_[EnemyType::BOSS] = my::MyLoadModel(bossEnemyFilePath.c_str());

	// 雑魚敵のインスタンスの作成
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
	// 存在していない敵の削除
	DeleteNotEnabledEnemy();

	if (boss_create_frame < time &&
		!isCreateBossEnemy_)
	{
		isCreateBossEnemy_ = true;
		pEnemies_.clear();

		pWarning_ = std::make_unique<Warning>(warning_ui_draw_frame);

		// updateをボス登場のupdateに切り替え
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
		// 敵ボスのインスタンス生成
		pEnemies_.push_back(
			std::make_shared<BossEnemy>(modelHandleTable_[EnemyType::BOSS],
				pPlayer_,
				pLazerManager_));

		updateFunc_ = &EnemyManager::NormalUpdate;
	}
}

void EnemyManager::DeleteNotEnabledEnemy()
{
	// 不要になった敵の削除
	pEnemies_.remove_if([](std::shared_ptr<EnemyBase> enemy) { return !enemy->IsEnabled(); });
}

void EnemyManager::NormalEnemyEntry()
{
	// TODO : エディターを作る or 外部ファイル化(パターンを複数作る) or Mayaやblenderを使ってフレームを配置してそこを目指す的な

	//// ファイル情報の読み込み(読み込みに失敗したら止める)
	//std::ifstream ifs("Data/Csv/Enemy.csv");
	//assert(ifs);

	//// csvデータを1行ずつ読み取る
	//std::vector<EnemyAIData> dataTable;
	//int index = 0;
	//bool isFirst = false;
	//std::string line;
	//while (getline(ifs, line))
	//{
	//	// 1行目は読み込まない
	//	// 1行目には項目が書いてあるため
	//	if (!isFirst)
	//	{
	//		isFirst = true;
	//		continue;
	//	}

	//	// csvデータ１行を','で複数の文字列に変換
	//	std::vector<std::string> strvec = StringManager::GetInstance().SplitString(line, ',');

	//	// csvデータから取得したデータからフォントハンドルの作成して格納
	//	EnemyAIData data{};
	//	
	//	// 初期位置の保存
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
	//		// 目的地の保存
	//		pos.x = std::stof(strvec[index]);
	//		index++;
	//		pos.y = std::stof(strvec[index]);
	//		index++;
	//		pos.z = std::stof(strvec[index]);
	//		index++;
	//		data.a.goalPos = pos;

	//		// スピードの保存
	//		data.a.speed = std::stof(strvec[index]);
	//		index++;

	//		// ショットするかの保存
	//		data.a.isShot = std::stoi(strvec[index]);
	//		index++;

	//		// 待機時間の保存
	//		data.a.idleTime = std::stoi(strvec[index]);
	//		index++;

	//		std::advance(itr, index);
	//	}
	//	dataTable.push_back(data);
	//}

	std::vector<NormalEnemyAIData> table;

	// 初期位置
	VECTOR initPos = { -2520, 540, 400 };

	// 敵1の行動1
	NormalEnemyAIData data{};
	data.goalPos = { -700, 300, 800 };
	data.speed = 15.0f;
	data.idleTime = 180.0f;
	data.isShot = true;
	table.push_back(data);

	// 敵1の行動2
	data.goalPos = initPos;
	data.speed = 15.0f;
	data.idleTime = 0.0f;
	data.isShot = false;
	table.push_back(data);

	// 敵1のインスタンスの作成
	pEnemies_.push_back(
		std::make_shared<NormalEnemy>(
		modelHandleTable_[EnemyType::NOMAL], 
		pPlayer_, 
		pLazerManager_,
		initPos,
		table));

	std::vector<NormalEnemyAIData> table2;
	initPos = { -2520, -540, 400 };

	// 敵2の行動1
	data.goalPos = { -700, -300, 800 };
	data.speed = 15.0f;
	data.idleTime = 180.0f;
	data.isShot = true;
	table2.push_back(data);

	// 敵2の行動2
	data.goalPos = initPos;
	data.speed = 15.0f;
	data.idleTime = 0.0f;
	data.isShot = false;
	table2.push_back(data);

	// 敵2のインスタンスの作成
	pEnemies_.push_back(
		std::make_shared<NormalEnemy>(
			modelHandleTable_[EnemyType::NOMAL],
			pPlayer_,
			pLazerManager_,
			initPos,
			table2));
}