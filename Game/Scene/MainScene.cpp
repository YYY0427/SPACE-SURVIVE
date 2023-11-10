#include "MainScene.h"
#include "TitleScene.h"
#include "PauseScene.h"
#include "SceneManager.h"
#include "../common.h"
#include "../Util/InputState.h"
#include "../Util/Effekseer3DEffectManager.h"
#include "../Util/SoundManager.h"
#include "../Util/Debug.h"
#include "../Camera.h"
#include "../Player.h"
#include "../SkyDome.h"
#include "../Rock/RockManager.h"
#include "../Rock/RockBase.h"
#include <DxLib.h>

// コンストラクタ
MainScene::MainScene(SceneManager& manager) :
	SceneBase(manager),
	updateFunc_(&MainScene::NormalUpdate)
{
//	// オブジェクトの配置データの読み込み
//	pDataReader_ = std::make_shared<DataReaderFromUnity>();
//	pDataReader_->LoadUnityGameObjectData();
//
//	pPlayer_ = std::make_shared<Player>(pDataReader_->GetPlayerData());
//	pRockManager_ = std::make_shared<RockManager>(pDataReader_->GetRockData(), pPlayer_);
//	pCamera_ = std::make_shared<Camera>(pPlayer_);
////	pSkyDome_ = std::make_shared<SkyDome>(pPlayer_);
//
//	// コンストラクタで渡せないポインタの設定
//	pPlayer_->SetCameraPointer(pCamera_);
}

// デストラクタ
MainScene::~MainScene()
{
	// 処理なし
}

// メンバ関数ポインタの更新
void MainScene::Update()
{
	(this->*updateFunc_)();
}

// 通常の更新
void MainScene::NormalUpdate()
{
//	// 各クラスの更新
////	pSkyDome_->Update();
//	pCamera_->Update();
//	pPlayer_->Update();
//	pRockManager_->Update();
//
//	// 敵とぶつかったらゲームオーバー
//	for (auto& rock : pRockManager_->GetRocks())
//	{
//		MV1_COLL_RESULT_POLY_DIM result = MV1CollCheck_Sphere(rock->GetModelHandle(), -1, pPlayer_->GetPos(), pPlayer_->GetCollsionRadius());
//		if (result.HitNum > 0)
//		{
//			// Updateをゲームオーバー時のUpdateに変更
//			updateFunc_ = &MainScene::GameOverUpdate;
//		}
//	}

	// ポーズ画面に遷移
	if (InputState::IsTriggered(InputType::PAUSE))
	{
		// フェードアウト開始
		StartFadeOut(200, 64);
	}

	// フェードが終わり次第シーン遷移
	if (IsStartFadeOutAfterFadingOut())
	{
		// エフェクトを全て止める
		Effekseer3DEffectManager::GetInstance().StopAllEffect();

		// PushSceneするのでシーンが残るためフェードインの設定
		StartFadeIn();

		// シーン遷移
		manager_.PushScene(new PauseScene(manager_));
		return;
	}

	// フェードの更新
	UpdateFade();
}

void MainScene::GameOverUpdate()
{
	// フェードアウトが終わり次第シーン遷移
	if (IsStartFadeOutAfterFadingOut())
	{
		manager_.ChangeScene(new TitleScene(manager_));
		return;
	}

	// カメラの更新
//	pCamera_->Update();

	// プレイヤーのゲームオーバー時の更新が終了かつフェードしてなかったらフェードアウト開始
	if (pPlayer_->OnDamageUpdate() && !IsFadeing())
	{
		// フェードアウトの開始
		StartFadeOut(255);
	}
	// フェードの更新
	UpdateFade();
}

// 描画
void MainScene::Draw()
{
	// 現在のシーンのテキスト表示
	Debug::Log("MainScene");

	// 各クラスの描画
//	pSkyDome_->Draw();
//	pRockManager_->Draw();
//	pPlayer_->Draw();

	// フェードの描画
	DrawFade(true);

	// モザイクフェードの描画
	DrawGaussFade(true);
}