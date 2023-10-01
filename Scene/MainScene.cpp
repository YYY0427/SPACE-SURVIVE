#include "MainScene.h"
#include "TitleScene.h"
#include "PauseScene.h"
#include "SceneManager.h"
#include "../common.h"
#include "../Util/InputState.h"
#include "../Util/Effekseer3DEffectManager.h"
#include "../Util/SoundManager.h"
#include "../Camera.h"
#include "../Player.h"
#include "../SkyDome.h"
#include "../EnemyManager.h"
#include "../Enemy.h"
#include <DxLib.h>

// コンストラクタ
MainScene::MainScene(SceneManager& manager) :
	Scene(manager),
	updateFunc_(&MainScene::NormalUpdate)
{
	// インスタンス作成
	pPlayer_ = std::make_shared<Player>();
	pCamera_ = std::make_shared<Camera>(pPlayer_);
//	pSkyDome_ = std::make_shared<SkyDome>(pPlayer_);
	pEnemyManager_ = std::make_shared<EnemyManager>(pPlayer_);

	// コンストラクタで渡せないポインタの設定
	pPlayer_->SetCameraPointer(pCamera_);
}

// デストラクタ
MainScene::~MainScene()
{
}

// メンバ関数ポインタの更新
void MainScene::Update()
{
	(this->*updateFunc_)();
}

// 通常の更新
void MainScene::NormalUpdate()
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
			updateFunc_ = &MainScene::GameOverUpdate;
		}
	}

	// ポーズ画面に遷移
	if (InputState::IsTriggered(InputType::PAUSE))
	{
		// フェードアウト開始
		StartFadeOut(200, 64);
	}
	// フェードが終わり次第シーン遷移
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

void MainScene::GameOverUpdate()
{
	// フェードアウトが終わり次第シーン遷移
	if (IsStartFadeOutAfterFadingOut())
	{
		manager_.ChangeScene(new TitleScene(manager_));
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

// 描画
void MainScene::Draw()
{
	// 現在のシーンのテキスト表示
	DrawString(0, 0, "MainScene", 0xffffff, true);

	// 各クラスの描画
//	pSkyDome_->Draw();
	pEnemyManager_->Draw();
	pPlayer_->Draw();

	// フェードの描画
	DrawFade(true);

	// モザイクフェードの描画
	DrawGaussFade(true);
}