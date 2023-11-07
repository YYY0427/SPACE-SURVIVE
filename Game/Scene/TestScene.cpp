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
#include "../Rock/RockManager.h"
#include "../Rock/RockBase.h"
#include "../Planet/PlanetManager.h"
#include "../Planet/PlanetBase.h"
#include "../Road.h"
#include "../RoadManager.h"
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
	updateFunc_(&TestScene::NormalUpdate),
	nextRoad_(1)
{
	// オブジェクトの配置データの読み込み
	pDataReader_ = std::make_shared<DataReaderFromUnity>();
	pDataReader_->LoadUnityGameObjectData();

	// 読み込んだ配置データからオブジェクトのインスタンスの生成
	pRoadManager_ = std::make_shared<RoadManager>(pDataReader_->GetDataType("Stage"));
	pLazerManager_ = std::make_shared<LazerManager>();
	pPlayer_ = std::make_shared<Player>(pDataReader_->GetDataType("Player2").front());
	pEnemyManager_ = std::make_shared<EnemyManager>(pPlayer_, pLazerManager_, pDataReader_->GetDataType("BossEnemy").front(), pDataReader_->GetDataType("NormalEnemy"));
	pRockManager_ = std::make_shared<RockManager>(pDataReader_->GetDataType("Rock"), pDataReader_->GetDataType("Meteor"));
	pPlanetManager_ = std::make_shared<PlanetManager>(pDataReader_->GetDataType("Sun"), pDataReader_->GetDataType("Earth"));
	pCamera_ = std::make_shared<Camera>(pPlayer_, pDataReader_->GetDataType("Camera").front());
	pSkyDome_ = std::make_shared<SkyDome>();
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
	pRoadManager_->Draw();
	pRockManager_->Draw();
	pPlanetManager_->Draw();
	pEnemyManager_->Draw();
	pLazerManager_->Draw();
	pPlayer_->Draw();

	// 現在のシーンのテキスト表示
	Debug::Log("TestScene");

#ifdef _DEBUG
	// 地面の角の位置表示
	for (auto& road : pRoadManager_->GetRoads())
	{
		VECTOR leftTop = road->GetVertex()[0].pos;
		VECTOR rightTop = road->GetVertex()[1].pos;
		VECTOR leftBottom = road->GetVertex()[2].pos;
		VECTOR rightBottom = road->GetVertex()[3].pos;

		DrawSphere3D(leftTop, 32, 8, 0xff0000, 0xff0000, 0xff0000);
		DrawSphere3D(rightTop, 32, 8, 0xff0000, 0xff0000, 0xff0000);
		DrawSphere3D(leftBottom, 32, 8, 0xff0000, 0xff0000, 0xff0000);
		DrawSphere3D(rightBottom, 32, 8, 0xff0000, 0xff0000, 0xff0000);
	}
#endif

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


	// 各クラスの更新
	pSkyDome_->Update(pPlayer_->GetPos());
	pRoadManager_->Update(pPlayer_->GetPos());
	pPlayer_->Update(pCamera_->GetCameraYaw());
	pLazerManager_->Update();
	pEnemyManager_->Update();
	pRockManager_->Update();
	pPlanetManager_->Update();
	pCamera_->Update();

	// 道の無限スクロール処理
	RoadInfiniteScroll();

	// プレイヤーの落下処理
	PlayerFallProcess();

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

		MV1_COLL_RESULT_POLY_DIM result = MV1CollCheck_Triangle(lazer.pLazer->GetModelHandle(), -1, leftTop, rightTop, leftBottom);
		MV1_COLL_RESULT_POLY_DIM result2 = MV1CollCheck_Triangle(lazer.pLazer->GetModelHandle(), -1, rightBottom, leftBottom, rightTop);

		// 1つでもポリゴンと当たっていたら
		if (result.HitNum > 0 || result2.HitNum > 0)
		{
			VECTOR firePos{};
			result.HitNum > 0 ? 
				firePos = result.Dim->HitPosition : 
				firePos = result2.Dim->HitPosition;

			lazer.pLazer->Refrect(pPlayer_->GetShield()->GetPos());
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

		// 反射前のレーザーとしか当たり判定を行わない
		if (!laser.pLazer->GetIsRefrect()) continue;

		for (auto& enemy : pEnemyManager_->GetEnemies())
		{
			MV1_COLL_RESULT_POLY_DIM result = MV1CollCheck_Sphere(laser.pLazer->GetModelHandle(), -1, enemy->GetPos(), enemy->GetCollisionRadius());

			if (result.HitNum > 0)
			{
				pEnemyManager_->OnDamage(10.0f);
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

	// ゲームクリア
	if (pEnemyManager_->GetIsRepel())
	{
		// フェードアウト開始
		StartFadeOut(255, 10);
		item_ = SceneItem::TITLE;
	}
	// ゲームオーバー
	else if (!pPlayer_->IsLive())
	{
		// フェードアウト開始
		StartFadeOut(255, 10);
		item_ = SceneItem::TITLE;
	}

	// フェードの更新
	UpdateFade();
}

// 岩と衝突時の更新
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
		bool isEnd = pPlayer_->CollisionRockUpdate();

		if (isEnd)
		{
			// フェードアウトの開始
			StartFadeOut(255);
		}
	}

	// カメラの更新
	pCamera_->Update();

	pSkyDome_->Update(pPlayer_->GetPos());
	pRoadManager_->Update(pPlayer_->GetPos());
	pLazerManager_->Update();

	// 道の無限スクロール処理
	RoadInfiniteScroll();

	// フェードの更新
	UpdateFade();
}

// プレイヤー落下死亡時の更新
void TestScene::DeathFallPlayerUpdate()
{
	// 全てのエフェクトの削除
	Effekseer3DEffectManager::GetInstance().StopAllEffect();

	// フェードアウトが終了したらリスポーン
	if (IsStartFadeOutAfterFadingOut())
	{
		// ダメージ処理
		pPlayer_->OnDamage();

		// プレイヤーが生きていたらリスポーン
		if (pPlayer_->IsLive())
		{
			// プレイヤーから1番近い道の座標の取得
			VECTOR restartPos = pRoadManager_->GetClosestRoadPos(pPlayer_->GetPos());

			// プレイヤーのリスポーン処理
			pPlayer_->Respawn(restartPos);

			// フェードインの開始
			StartFadeIn();

			// Updateを通常のUpdateに変更
			updateFunc_ = &TestScene::NormalUpdate;
		}
		// 死んでいたらシーン遷移
		else
		{
			manager_.ChangeScene(new DebugScene(manager_));
		}
		return;
	}

	// 1度だけフェードアウト処理を実行
	if (!IsFadeing())
	{
		// フェードアウトの開始
		StartFadeOut(255);
	}

	// フェードの更新
	UpdateFade();
}

// プレイヤーの落下処理をまとめたもの
void TestScene::PlayerFallProcess()
{
	// プレイヤーが道の上にいるか
	bool isPlayerOnTheRoad = JudgePlayerOnTheRoad();

#if false
	if (!isPlayerOnTheRoad)
	{
		pPlayer_->A();
	}
#else
	// 道からプレイヤーまでの距離が特定の距離を超えているか
	bool isOverLimitPlayerHeight = OverLimitPlayerHeight();

	// プレイヤーが道の上になかったらプレイヤーが落下
	if (!isPlayerOnTheRoad)
	{
		pPlayer_->Fall(normal_player_fall_speed);
	}
	// 道からプレイヤーまでの距離が特定の距離を超えていたらプレイヤーが落下
	if (isOverLimitPlayerHeight)
	{
		pPlayer_->Fall(special_player_fall_speed);
	}

	// プレイヤーが落下死亡判定の高さまで落ちたかどうか
	if (pPlayer_->IsDeathJudgHeight())
	{
		// 落ちていたらUpdateをプレイヤー落下死亡時のUpdateに変更
		updateFunc_ = &TestScene::DeathFallPlayerUpdate;
		return;
	}
#endif
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

// 道の無限スクロール
void TestScene::RoadInfiniteScroll()
{
	HITRESULT_LINE firstRoadResult{}, firstRoadResult2{};
	HITRESULT_LINE secondRoadResult{}, secondRoadResult2{};

	// 1つ目の道、2つ目の道のインスタンスの取得
	auto firstRoad = pRoadManager_->GetRoads()[0];
	auto secondRoad = pRoadManager_->GetRoads()[1];

	// 画像の高さの取得
	float imgHeight = firstRoad->GetImageHeight();

	// 1つ目の道、2つ目の道の位置座標の取得
	VECTOR firstRoadPos = firstRoad->GetPos();
	VECTOR secondRoadPos = secondRoad->GetPos();

	// プレイヤーから下に伸びる線と道の当たり判定
	CollisionRoadAndPlayer(firstRoad, firstRoadResult, firstRoadResult2);
	CollisionRoadAndPlayer(secondRoad, secondRoadResult, secondRoadResult2);

	// 道と当たったかどうか
	bool isHitFirstRoad = firstRoadResult.HitFlag || firstRoadResult2.HitFlag;
	bool isHitSecondRoad = secondRoadResult.HitFlag || secondRoadResult2.HitFlag;

	// 2つ目の道の上に乗ったら1つ目の道を2つ目の道の前に座標変更
	// 道を無限スクロールさせるため
	if (nextRoad_ == 1 && isHitSecondRoad)
	{
		firstRoad->SetPos({ secondRoadPos.x, secondRoadPos.y, secondRoad->GetPos().z + imgHeight * 2 });
		nextRoad_ = 0;
	}
	else if (nextRoad_ == 0 && isHitFirstRoad)
	{
		secondRoad->SetPos({ firstRoadPos.x, firstRoadPos.y, firstRoad->GetPos().z + imgHeight * 2 });
		nextRoad_ = 1;
	}
}

// プレイヤーから伸ばした線とその線に当たった道までの距離が特定の距離を超えているか
bool TestScene::OverLimitPlayerHeight()
{
	// プレイヤーから下に伸びる線と道の当たり判定
	HITRESULT_LINE result{}, result2{};
	CollisionAllRoadAndPlayer(result, result2);

	// 1つでもポリゴンとプレイヤーから延ばした線が当たっていたらチェック
	if (result.HitFlag || result2.HitFlag)
	{
		// プレイヤーから当たった道までの高さの距離の取得
		float distanceFromPlayerToRoad = 0.0f;
		(result.HitFlag) ?
			(distanceFromPlayerToRoad = fabs((result.Position.y - pPlayer_->GetPos().y))) :
			(distanceFromPlayerToRoad = fabs((result2.Position.y - pPlayer_->GetPos().y)));

		// 高さの距離が特定の距離を超えていたらtrue
		if (rise_possible_height < distanceFromPlayerToRoad)
		{
			return true;
		}
	}
	return false;
}

// プレイヤーが道の上にいるか判定
bool TestScene::JudgePlayerOnTheRoad()
{
	// プレイヤーから下に伸びる線と道の当たり判定
	HITRESULT_LINE result{}, result2{};
	CollisionAllRoadAndPlayer(result, result2);

	// 1つでもポリゴンとプレイヤーから延ばした線が当たっていたら道の上にいる判定
	if (result.HitFlag || result2.HitFlag)
	{
		return true;
	}
	return false;
}

// プレイヤーから下に伸びている線と道との当たり判定
void TestScene::CollisionRoadAndPlayer(std::shared_ptr<Road> pRoad, HITRESULT_LINE& result, HITRESULT_LINE& result2)
{
	// プレイヤーの現在位置から下方向に線を伸ばして地面のポリゴンと当たっているか確かめる
	VECTOR leftTop = pRoad->GetVertex()[0].pos;
	VECTOR rightTop = pRoad->GetVertex()[1].pos;
	VECTOR leftBottom = pRoad->GetVertex()[2].pos;
	VECTOR rightBottom = pRoad->GetVertex()[3].pos;

	// ラインを伸ばす開始位置
	VECTOR startLinePos = pPlayer_->GetPos();

	// ラインの終了位置
	VECTOR endLinePos = VGet(pPlayer_->GetPos().x, pPlayer_->GetPos().y - 10000.0f, pPlayer_->GetPos().z);

	// 道は2つのポリゴンで描画しているので2つともチェック
	result = HitCheck_Line_Triangle(startLinePos, endLinePos, leftTop, rightTop, leftBottom);
	result2 = HitCheck_Line_Triangle(startLinePos, endLinePos, rightBottom, leftBottom, rightTop);
}

void TestScene::CollisionAllRoadAndPlayer(HITRESULT_LINE& result, HITRESULT_LINE& result2)
{
	// 全ての道の数チェック
	for (auto& road : pRoadManager_->GetRoads())
	{
		CollisionRoadAndPlayer(road, result, result2);

		// 1つでもポリゴンとプレイヤーから延ばした線が当たっていたら処理終了
		// 当たった後も処理を続けると結果が書き変わってしまうため
		if (result.HitFlag || result2.HitFlag)
		{
			break;
		}
	}
}
