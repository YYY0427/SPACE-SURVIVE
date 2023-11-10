#include "TestScene.h"
#include "SceneManager.h"
#include "TitleScene.h"
#include "DebugScene.h"
#include "PauseScene.h"
#include "../Util/DrawFunctions.h"
#include "../Util/Effekseer3DEffectManager.h"
#include "../Util/SoundManager.h"
#include "../Util/InputState.h"
#include "../Util/DataReaderFromUnity.h"
#include "../Util/Debug.h"
#include "../Camera.h"
#include "../Player.h"
#include "../SkyDome.h"
#include "../Planet/PlanetManager.h"
#include "../Planet/PlanetBase.h"
#include "../Enemy/EnemyManager.h"
#include "../Laser/LazerManager.h"
#include "../Shield.h"
#include "../common.h"

namespace
{
	// 道からどのくらいの高さまでプレイヤーが上昇できるか
	constexpr float rise_possible_height = 2000.0f;

	// 通常のプレイヤーの落下速度
	constexpr float normal_player_fall_speed = 20.0f;

	// プレイヤーの高さが特定の高さよりも高くなった時の落下速度
	constexpr float special_player_fall_speed = 40.0f;
}

// コンストラクタ
TestScene::TestScene(SceneManager& manager) :
	SceneBase(manager),
	updateFunc_(&TestScene::NormalUpdate)
{
	// オブジェクトの配置データの読み込み
	DataReaderFromUnity::GetInstance().LoadUnityGameObjectData("Data/ObjectData.dat");

	// 読み込んだ配置データからオブジェクトのインスタンスの生成
	pLazerManager_ = std::make_shared<LazerManager>();
	pPlayer_ = std::make_shared<Player>();
	pEnemyManager_ = std::make_shared<EnemyManager>(pPlayer_, pLazerManager_);
	pPlanetManager_ = std::make_shared<PlanetManager>();
	pCamera_ = std::make_shared<Camera>(pPlayer_);
	pSkyDome_ = std::make_shared<SkyDome>(pPlayer_->GetPos());

	/*Effekseer3DEffectManager::GetInstance().PlayEffectLoop(
		windEffectH_, 
		EffectID::wind, 
		{ pPlayer_->GetPos().x, pPlayer_->GetPos().y, pPlayer_->GetPos().z + 500.0f},
		200.0f,
		0.7f,
		{ 0.0f, -DX_PI_F / 2, 0.0f});*/
}

//  デストラクタ
TestScene::~TestScene()
{
	// 全てのエフェクトの削除
	Effekseer3DEffectManager::GetInstance().DeleteAllEffect();
}

// メンバ関数ポインタの更新
void TestScene::Update()
{
	(this->*updateFunc_)();
}

// 描画
void TestScene::Draw()
{
	// 各クラスの描画
	pSkyDome_->Draw();
	pPlanetManager_->Draw();
	pEnemyManager_->Draw();
	pLazerManager_->Draw();
	pPlayer_->Draw();

	// 現在のシーンのテキスト表示
	Debug::Log("TestScene");

	// フェードの描画
	DrawFade(true);

	// モザイクフェードの描画
	DrawGaussFade(true);
}

// 通常の更新
void TestScene::NormalUpdate()
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
	pSkyDome_->Update(pPlayer_->GetMoveVecZ());
	pPlayer_->Update(pCamera_->GetCameraYaw());
	pEnemyManager_->Update();
	pLazerManager_->Update(pPlayer_->GetMoveVecZ());
	pPlanetManager_->Update();
	pCamera_->Update();

	// レーザーとシールドの当たり判定
	for (auto& lazer : pLazerManager_->GetLazeres())
	{
		// シールドを出していなかったら判定を行わない
		if (!pPlayer_->GetShield()->GetIsShield()) continue;

		// 反射可能なレーザー以外の場合は判定を行わない
		if (lazer.type != LazerType::NORMAL) continue;
		
		// 反射後のレーザーの場合は当たり判定を行わない
		if (lazer.pLazer->GetIsRefrect()) continue;

		VECTOR leftTop = pPlayer_->GetShield()->GetVertex()[0].pos;
		VECTOR rightTop = pPlayer_->GetShield()->GetVertex()[1].pos;
		VECTOR leftBottom = pPlayer_->GetShield()->GetVertex()[2].pos;
		VECTOR rightBottom = pPlayer_->GetShield()->GetVertex()[3].pos;

		// シールドは二つのポリゴンからできてるので二つのポリゴンともチェック
		MV1_COLL_RESULT_POLY_DIM result = MV1CollCheck_Triangle(lazer.pLazer->GetModelHandle(), -1, leftTop, rightTop, leftBottom);
		MV1_COLL_RESULT_POLY_DIM result2 = MV1CollCheck_Triangle(lazer.pLazer->GetModelHandle(), -1, rightBottom, leftBottom, rightTop);

		// 1つでもポリゴンと当たっていたら
		if (result.HitNum > 0 || result2.HitNum > 0)
		{
			// シールドの法線情報
			VECTOR shieldNorm = pPlayer_->GetShield()->GetVertex()[0].norm;

			// レーザーを反射
			lazer.pLazer->Refrect(pPlayer_->GetShield()->GetPos(), shieldNorm);
		}

		// 当たり判定情報の後始末
		MV1CollResultPolyDimTerminate(result);
		MV1CollResultPolyDimTerminate(result2);
	}

	// レーザーと敵の当たり判定
	for (auto& laser : pLazerManager_->GetLazeres())
	{
		// 反射可能なレーザー以外の場合は判定を行わない
		if (laser.type != LazerType::NORMAL) continue;

		// 反射後のレーザーとしか当たり判定を行わない
		if (!laser.pLazer->GetIsRefrect()) continue;

		// 全ての敵をチェック
		for (auto& enemy : pEnemyManager_->GetEnemies())
		{
			// レーザーのモデルと敵を球体にみたてて当たり判定
			MV1_COLL_RESULT_POLY_DIM result = MV1CollCheck_Sphere(laser.pLazer->GetModelHandle(), -1, enemy->GetPos(), enemy->GetCollisionRadius());

			// ポリゴン一つにでも当たっていたら
			if (result.HitNum > 0)
			{
				enemy->OnDamage(10.0f, result.Dim->HitPosition);

				// 何回も当たるのを防ぐため一回当たったらレーザーを削除
				laser.pLazer->Delete();
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

		// 反射後のレーザーの場合プレイヤーと当たり判定を行わない
		if (laser.pLazer->GetIsRefrect()) continue;

		// レーザーとプレイヤーの当たり判定
		MV1_COLL_RESULT_POLY_DIM result = MV1CollCheck_Sphere(laser.pLazer->GetModelHandle(), -1, pPlayer_->GetPos(), pPlayer_->GetCollsionRadius());

		// 1つでもポリゴンと当たっていたら
		if (result.HitNum > 0)
		{
			// Updateをゲームオーバー時のUpdateに変更
			updateFunc_ = &TestScene::CollisionRockUpdate;

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

void TestScene::CollisionRockUpdate()
{
	// フェードアウトが終わり次第シーン遷移
	if (IsStartFadeOutAfterFadingOut())
	{
		if (pPlayer_->IsLive())
		{
			StartFadeIn();

			// Updateを通常時のUpdateに変更
			updateFunc_ = &TestScene::NormalUpdate;
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
	pPlayer_->Scroll();
	pCamera_->Update();
	pSkyDome_->Update(pPlayer_->GetMoveVecZ());
	pLazerManager_->Update(pPlayer_->GetMoveVecZ());
	pEnemyManager_->Update();

	// フェードの更新
	UpdateFade();
}