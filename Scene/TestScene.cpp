#include "TestScene.h"
#include "SceneManager.h"
#include "DebugScene.h"
#include "PauseScene.h"
#include "../Util/Effekseer3DEffectManager.h"
#include "../Util/SoundManager.h"
#include "../Util/InputState.h"
#include "../Util/DataReaderFromUnity.h"
#include "../Camera.h"
#include "../Player.h"
#include "../SkyDome.h"
#include "../EnemyManager.h"
#include "../Enemy.h"
#include "../common.h"

// コンストラクタ
TestScene::TestScene(SceneManager& manager) :
	Scene(manager),
	updateFunc_(&TestScene::NormalUpdate)
{
	// インスタンス作成
	pDataReader_ = std::make_shared<DataReaderFromUnity>();
	pPlayer_ = std::make_shared<Player>();
	pCamera_ = std::make_shared<Camera>(pPlayer_);
//	pSkyDome_ = std::make_shared<SkyDome>(pPlayer_);
	pEnemyManager_ = std::make_shared<EnemyManager>(pPlayer_);
}

//  デストラクタ
TestScene::~TestScene()
{
	// 処理なし
}

// 初期化
void TestScene::Init()
{
	// コンストラクタで渡せないポインタの設定
	pPlayer_->SetCameraPointer(pCamera_);

	// オブジェクトの配置データの読み込み
	pDataReader_->LoadUnityGameObjectData();
}

// メンバ関数ポインタの更新
void TestScene::Update()
{
	(this->*updateFunc_)();
}

// 描画
void TestScene::Draw()
{
	// 背景を白に変更
	DrawBox(0, 0, common::screen_width, common::screen_height, 0xcccccc, true);

	// 現在のシーンのテキスト表示
	DrawString(0, 0, "TestScene", 0xffffff, true);

	// 各クラスの描画
//	pSkyDome_->Draw();
	GroundLineDraw();
	pEnemyManager_->Draw();
	pPlayer_->Draw();

	int x = 200, y = 200;
	for (const auto& data : pDataReader_->GetEnemyData())
	{
		DrawFormatString(x, y, 0x000000, "%s = pos {%.2f, %.2f, %.2f}, rot {%.2f, %.2f, %.2f}", data.name.c_str(), data.pos.x, data.pos.y, data.pos.z, data.rot.x, data.rot.y, data.rot.z);
		y += 16;
	}

	DrawFormatString(200, y, 0x000000, "%s = pos {%.2f, %.2f, %.2f}, rot {%.2f, %.2f, %.2f}", pDataReader_->GetPlayerData().name.c_str(), pDataReader_->GetPlayerData().pos.x, pDataReader_->GetPlayerData().pos.y, pDataReader_->GetPlayerData().pos.z, pDataReader_->GetPlayerData().rot.x, pDataReader_->GetPlayerData().rot.y, pDataReader_->GetPlayerData().rot.z);

	// フェードの描画
	DrawFade(true);

	DrawGaussFade(true);
}

// 通常の更新
void TestScene::NormalUpdate()
{
	// 各クラスの更新
//	pSkyDome_->Update();
	pCamera_->Update();
	pPlayer_->Update();
	pEnemyManager_->Update();

	// 敵とぶつかったらゲームオーバー
	for (auto& enemies : pEnemyManager_->GetEnemies())
	{
		MV1_COLL_RESULT_POLY_DIM result = MV1CollCheck_Sphere(enemies->GetModelHandle(), -1, pPlayer_->GetPos(), pPlayer_->GetCollsionRadius());
		if (result.HitNum > 0)
		{
			// Updateをゲームオーバー時のUpdateに変更
			updateFunc_ = &TestScene::GameOverUpdate;
		}
	}

	// ポーズ画面に遷移
	if (InputState::IsTriggered(InputType::PAUSE))
	{
		StartFadeOut(200, 64);
	}
	if (IsStartFadeOutAfterFadingOut())
	{
		Effekseer3DEffectManager::GetInstance().StopAllEffect();
		StartFadeIn();
		manager_.PushScene(new PauseScene(manager_));
		return;
	}

	// フェードの更新
	UpdateFade();
}

// ゲームオーバー時の更新
void TestScene::GameOverUpdate()
{
	// フェードアウトが終わり次第シーン遷移
	if (IsStartFadeOutAfterFadingOut())
	{
		manager_.ChangeScene(new DebugScene(manager_));
		return;
	}

	// カメラの更新
	pCamera_->Update();

	// プレイヤーのゲームオーバー時の更新が終了かつフェードしてなかったらフェードアウト開始
	if (pPlayer_->GameOverUpdate() && !IsFadeing())
	{
		// フェードアウトの開始
		StartFadeOut(255);
	}
	// フェードの更新
	UpdateFade();
}

// 地面の線の描画
void TestScene::GroundLineDraw()
{
	float lineAreaSize = 10000.0f;
	int lineNum = 50;

	VECTOR pos1 = VGet(-lineAreaSize / 2.0f, 0.0f, -lineAreaSize / 2.0f);
	VECTOR pos2 = VGet(-lineAreaSize / 2.0f, 0.0f, lineAreaSize / 2.0f);
	for (int i = 0; i <= lineNum; i++)
	{
		DrawLine3D(pos1, pos2, GetColor(0, 0, 0));
		pos1.x += lineAreaSize / lineNum;
		pos2.x += lineAreaSize / lineNum;
	}

	pos1 = VGet(-lineAreaSize / 2.0f, 0.0f, -lineAreaSize / 2.0f);
	pos2 = VGet(lineAreaSize / 2.0f, 0.0f, -lineAreaSize / 2.0f);
	for (int i = 0; i < lineNum; i++)
	{
		DrawLine3D(pos1, pos2, GetColor(0, 0, 0));
		pos1.z += lineAreaSize / lineNum;
		pos2.z += lineAreaSize / lineNum;
	}
}