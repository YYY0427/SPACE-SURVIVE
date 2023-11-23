#include "GameMainScene.h"
#include "SceneManager.h"
#include "TitleScene.h"
#include "DebugScene.h"
#include "PauseScene.h"
#include "../Util/Effekseer3DEffectManager.h"
#include "../Util/SoundManager.h"
#include "../Util/InputState.h"
#include "../Util/Debug.h"
#include "../Camera.h"
#include "../Player.h"
#include "../Planet/PlanetManager.h"
#include "../Planet/PlanetBase.h"
#include "../Enemy/EnemyManager.h"
#include "../Laser/LazerManager.h"
#include "../Background.h"
#include "../Shield.h"
#include "../common.h"

namespace
{
}

// コンストラクタ
GameMainScene::GameMainScene(SceneManager& manager) :
	SceneBase(manager),
	updateFunc_(&GameMainScene::NormalUpdate)
{
	// インスタンス生成
	pBackground_ = std::make_shared<Background>();
	pLazerManager_ = std::make_shared<LazerManager>();
	pPlayer_ = std::make_shared<Player>();
	pEnemyManager_ = std::make_shared<EnemyManager>(pPlayer_, pLazerManager_);
	pPlanetManager_ = std::make_shared<PlanetManager>();
	pCamera_ = std::make_shared<Camera>();
}

// デストラクタ
GameMainScene::~GameMainScene()
{
	// 全てのエフェクトの削除
	Effekseer3DEffectManager::GetInstance().DeleteAllEffect();
}

// メンバ関数ポインタの更新
void GameMainScene::Update()
{
	timer_.Update(1);
	
	(this->*updateFunc_)();
}

// 描画
void GameMainScene::Draw()
{
	// 各クラスの描画
	pBackground_->Draw();
	pPlanetManager_->Draw();
	pEnemyManager_->Draw();
	pPlayer_->Draw();
	pLazerManager_->Draw();

	// 各クラスのUIの描画
	pPlayer_->DrawUI();
	pEnemyManager_->DrawUI();

	// 現在のシーンのテキスト表示
	Debug::Log("GameMainScene");

	// フェードの描画
	DrawFade(true);

	// モザイクフェードの描画
	DrawGaussFade(true);
}

// 通常の更新
void GameMainScene::NormalUpdate()
{
	// フェードが終わり次第シーン遷移
	if (IsStartFadeOutAfterFadingOut())
	{
		switch (item_)
		{
			// ポーズシーンに遷移
		case SceneItem::PAUSE:
			manager_.PushScene(new PauseScene(manager_));
			break;

		case SceneItem::TITLE:
#ifdef _DEBUG
			manager_.ChangeScene(new DebugScene(manager_));
#else
			manager_.ChangeScene(new TitleScene(manager_));
#endif
			return;
		}
		// PushSceneするのでシーンが残るためフェードインの設定
		StartFadeIn();
		return;
	}

	// 更新
	pPlayer_->Update(pCamera_->GetCameraYaw());
	pEnemyManager_->Update(timer_.GetTime());
	pLazerManager_->Update();
	pPlanetManager_->Update();
	pCamera_->Update();

	if (pPlayer_->GetShield()->GetIsShield())
	{
		// レーザーとシールドの当たり判定
		for (auto& laser : pLazerManager_->GetLazeres())
		{
			// キューブレーザーは反射できないので飛ばす
			if (laser.type == LaserType::CUBE) continue;
			if (laser.type == LaserType::REFLECT) continue;

			VECTOR leftTop = pPlayer_->GetShield()->GetVertex()[0].pos;
			VECTOR rightTop = pPlayer_->GetShield()->GetVertex()[1].pos;
			VECTOR leftBottom = pPlayer_->GetShield()->GetVertex()[2].pos;
			VECTOR rightBottom = pPlayer_->GetShield()->GetVertex()[3].pos;

			// シールドは二つのポリゴンからできてるので二つのポリゴンともチェック
			MV1_COLL_RESULT_POLY_DIM result = MV1CollCheck_Triangle(laser.pLaser->GetModelHandle(), -1, leftTop, rightTop, leftBottom);
			MV1_COLL_RESULT_POLY_DIM result2 = MV1CollCheck_Triangle(laser.pLaser->GetModelHandle(), -1, rightBottom, leftBottom, rightTop);

			// 1つでもポリゴンと当たっていたら
			if (result.HitNum > 0 || result2.HitNum > 0)
			{
				laser.pLaser->SetIsReflect(true);

				// レーザーを止める
				laser.pLaser->Stop(pPlayer_->GetShield()->GetPos());

				// 反射レーザーを作成(既に作成されていた場合更新を行う)
				// シールドの法線情報
				VECTOR shieldNorm = pPlayer_->GetShield()->GetVertex()[0].norm;
				pLazerManager_->Reflect(pPlayer_->GetShield()->GetPos(), laser.pLaser->GetVec(), shieldNorm);
			}
			else
			{
				laser.pLaser->SetIsReflect(false);
			}

			// 当たり判定情報の後始末
			MV1CollResultPolyDimTerminate(result);
			MV1CollResultPolyDimTerminate(result2);
		}
	}

	// レーザーと敵の当たり判定
	for (auto& laser : pLazerManager_->GetLazeres())
	{
		// 反射可能なレーザー以外の場合は判定を行わない
		if (laser.type != LaserType::REFLECT) continue;

		// 全ての敵をチェック
		for (auto& enemy : pEnemyManager_->GetEnemies())
		{
			// レーザーのモデルと敵を球体にみたてて当たり判定
			MV1_COLL_RESULT_POLY_DIM result = MV1CollCheck_Sphere(laser.pLaser->GetModelHandle(), -1, enemy->GetPos(), enemy->GetCollisionRadius());

			// ポリゴン一つにでも当たっていたら
			if (result.HitNum > 0)
			{
				enemy->OnDamage(1.0f, result.Dim->HitPosition);
			}

			// 当たり判定情報の後始末
			MV1CollResultPolyDimTerminate(result);
		}
	}

	// レーザーとプレイヤーの当たり判定
	for (auto& laser : pLazerManager_->GetLazeres())
	{
		// 無敵時間中なら当たらない
		if (pPlayer_->IsUltimate()) continue;

		if (laser.type == LaserType::REFLECT) continue;

		// レーザーとプレイヤーの当たり判定
		MV1_COLL_RESULT_POLY_DIM result = MV1CollCheck_Sphere(laser.pLaser->GetModelHandle(), -1, pPlayer_->GetPos(), pPlayer_->GetCollsionRadius());

		// 1つでもポリゴンと当たっていたら
		if (result.HitNum > 0)
		{
			// Updateをゲームオーバー時のUpdateに変更
			updateFunc_ = &GameMainScene::CollisionRockUpdate;

			// 当たり判定情報の後始末
			MV1CollResultPolyDimTerminate(result);
			return;
		}

		// 当たり判定情報の後始末
		MV1CollResultPolyDimTerminate(result);
	}

	// ポーズ画面に遷移
	if (InputState::IsTriggered(InputType::PAUSE))
	{
		// フェードアウト開始
		StartFadeOut(200, 64);
		item_ = SceneItem::PAUSE;
	}

	// ゲームオーバー
	if (!pPlayer_->IsLive())
	{
		// フェードアウト開始
		StartFadeOut(255, 10);
		item_ = SceneItem::TITLE;
	}

	// フェードの更新
	UpdateFade();
}

void GameMainScene::CollisionRockUpdate()
{
	// フェードアウトが終わり次第シーン遷移
	if (IsStartFadeOutAfterFadingOut())
	{
		if (pPlayer_->IsLive())
		{
			StartFadeIn();

			// Updateを通常時のUpdateに変更
			updateFunc_ = &GameMainScene::NormalUpdate;
			return;
		}
	}

	if (!IsFadeing())
	{
		bool isEnd = pPlayer_->OnDamageUpdate();

		if (isEnd)
		{
			// フェードアウトの開始
			StartFadeOut(255);
		}
	}

	// 更新
	pCamera_->Update();
	pLazerManager_->Update();
	pEnemyManager_->Update(timer_.GetTime());

	// フェードの更新
	UpdateFade();
}