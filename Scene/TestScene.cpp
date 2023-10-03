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
#include "../Rock/RockManager.h"
#include "../Rock/RockBase.h"
#include "../common.h"

// コンストラクタ
TestScene::TestScene(SceneManager& manager) :
	Scene(manager),
	updateFunc_(&TestScene::NormalUpdate)
{
	// オブジェクトの配置データの読み込み
	pDataReader_ = std::make_shared<DataReaderFromUnity>();
	pDataReader_->LoadUnityGameObjectData();
	
	pPlayer_ = std::make_shared<Player>(pDataReader_->GetData().find("Player")->second.front());
	pRockManager_ = std::make_shared<RockManager>(pDataReader_->GetData().find("Rock")->second, pDataReader_->GetData().find("Meteor")->second, pPlayer_);
	pCamera_ = std::make_shared<Camera>(pPlayer_);
//	pSkyDome_ = std::make_shared<SkyDome>(pPlayer_);

	// コンストラクタで渡せないポインタの設定
	pPlayer_->SetCameraPointer(pCamera_);
}

//  デストラクタ
TestScene::~TestScene()
{
	// 処理なし
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
	pRockManager_->Draw();
	pPlayer_->Draw();

	int x = 200, y = 200;
	for (const auto& data : pDataReader_->GetRockData())
	{
		DrawFormatString(x, y, 0x000000, "%s = pos {%.2f, %.2f, %.2f}, rot {%.2f, %.2f, %.2f}, scale {%.2f, %.2f, %.2f}", data.name.c_str(), data.pos.x, data.pos.y, data.pos.z, data.rot.x, data.rot.y, data.rot.z, data.scale.x, data.scale.y, data.scale.z);
		y += 16;
	}
	auto player = pDataReader_->GetPlayerData();
	DrawFormatString(x,  y, 0x000000, "%s = pos {%.2f, %.2f, %.2f}, rot {%.2f, %.2f, %.2f}, scale {%.2f, %.2f, %.2f}", player.name.c_str(), player.pos.x, player.pos.y, player.pos.z, player.rot.x, player.rot.y, player.rot.z, player.scale.x, player.scale.y, player.scale.z);

	// フェードの描画
	DrawFade(true);

	// モザイクフェードの描画
	DrawGaussFade(true);
}

// 通常の更新
void TestScene::NormalUpdate()
{
	// 各クラスの更新
//	pSkyDome_->Update();
	pCamera_->Update();
	pPlayer_->Update();
	pRockManager_->Update();

	// 敵とぶつかったらゲームオーバー
	for (auto& enemies : pRockManager_->GetRocks())
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