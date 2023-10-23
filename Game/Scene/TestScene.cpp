#include "TestScene.h"
#include "SceneManager.h"
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
#include "../Image3D.h"
#include "../Image3DManager.h"
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
	pDataReader_ = std::make_shared<DataReaderFromUnity>();
	pDataReader_->LoadUnityGameObjectData();

	// 読み込んだ配置データからオブジェクトのインスタンスの生成
	pImg3DManager_ = std::make_shared<Image3DManager>(pDataReader_->GetDataType("Stage"));
	pPlayer_ = std::make_shared<Player>(pDataReader_->GetDataType("TestPlayer").front());
	pRockManager_ = std::make_shared<RockManager>(pDataReader_->GetDataType("Rock"), pDataReader_->GetDataType("Meteor"));
	pPlanetManager_ = std::make_shared<PlanetManager>(pDataReader_->GetDataType("Sun"), pDataReader_->GetDataType("Earth"));
	pCamera_ = std::make_shared<Camera>(pPlayer_);
	pSkyDome_ = std::make_shared<SkyDome>();

	// コンストラクタで渡せないポインタの設定
	pPlayer_->SetCameraPointer(pCamera_);

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
//	UpdateLoadAsync();

	(this->*updateFunc_)();
}

// 描画
void TestScene::Draw()
{
	// 各クラスの描画
	pSkyDome_->Draw();
	pImg3DManager_->Draw();
	pRockManager_->Draw();
	pPlanetManager_->Draw();
	pPlayer_->Draw();

	// 現在のシーンのテキスト表示
	Debug::Log("TestScene");

#ifdef _DEBUG
	// 地面の角の位置表示
	for (auto& road : pImg3DManager_->GetRoads())
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
	// 各クラスの更新
	pSkyDome_->Update(pPlayer_->GetPos());
	pCamera_->Update();
	pPlayer_->Update();
	pRockManager_->Update();
	pPlanetManager_->Update();

	// プレイヤーの落下処理
	PlayerFallProcess();

	// 岩とぶつかったらゲームオーバー
	for (auto& rocks : pRockManager_->GetRocks())
	{
		// 無敵時間中なら当たらない
		if (pPlayer_->IsUltimate()) continue;

		// 岩とプレイヤーの当たり判定チェック
		MV1_COLL_RESULT_POLY_DIM result = MV1CollCheck_Sphere(rocks->GetModelHandle(), -1, pPlayer_->GetPos(), pPlayer_->GetCollsionRadius());

		// 1つでもポリゴンと当たっていたら
		if (result.HitNum > 0)
		{
			// Updateをゲームオーバー時のUpdateに変更
			updateFunc_ = &TestScene::CollisionRockUpdate;
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
	}

	// フェードが終わり次第シーン遷移
	if (IsStartFadeOutAfterFadingOut())
	{
		// PushSceneするのでシーンが残るためフェードインの設定
		StartFadeIn();

		// ポーズシーンに遷移
		manager_.PushScene(new PauseScene(manager_));
		return;
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
		manager_.ChangeScene(new DebugScene(manager_));
		return;
	}

	// カメラの更新
	pCamera_->Update();

	if (pPlayer_->CollisionRockUpdate() && !IsFadeing())
	{
		// プレイヤーが生きているか
		if (pPlayer_->IsLive())
		{
			// Updateを通常時のUpdateに変更
			updateFunc_ = &TestScene::NormalUpdate;
			return;
		}
		else
		{
			// ゲームオーバー
			// フェードアウトの開始
			StartFadeOut(255);
		}
	}

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
			VECTOR restartPos = pImg3DManager_->GetClosestRoadPos(pPlayer_->GetPos());

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

	if (!isPlayerOnTheRoad)
	{
		pPlayer_->A();
	}

	// プレイヤーが落下死亡判定の高さまで落ちたかどうか
	if (pPlayer_->IsDeathJudgHeight())
	{
		// 落ちていたらUpdateをプレイヤー落下死亡時のUpdateに変更
		updateFunc_ = &TestScene::DeathFallPlayerUpdate;
		return;
	}
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

// プレイヤーが道の上にいるか判定
bool TestScene::JudgePlayerOnTheRoad()
{
	// プレイヤーから下に伸びる線と道の当たり判定
	HITRESULT_LINE result{}, result2{};
	CollisionRoadAndPlayer(result, result2);

	// 1つでもポリゴンとプレイヤーから延ばした線が当たっていたら道の上にいる判定
	if (result.HitFlag || result2.HitFlag)
	{
		return true;
	}
	return false;
}

// プレイヤーから下に伸びている線と道との当たり判定
void TestScene::CollisionRoadAndPlayer(HITRESULT_LINE& result, HITRESULT_LINE& result2)
{
	// 全ての道の数チェック
	for (auto& road : pImg3DManager_->GetRoads())
	{
		// プレイヤーの現在位置から下方向に線を伸ばして地面のポリゴンと当たっているか確かめる
		VECTOR leftTop = road->GetVertex()[0].pos;
		VECTOR rightTop = road->GetVertex()[1].pos;
		VECTOR leftBottom = road->GetVertex()[2].pos;
		VECTOR rightBottom = road->GetVertex()[3].pos;

		// ラインを伸ばす開始位置
		VECTOR startLinePos = pPlayer_->GetPos();

		// ラインの終了位置
		VECTOR endLinePos = VGet(pPlayer_->GetPos().x, pPlayer_->GetPos().y - 10000.0f, pPlayer_->GetPos().z);

		// 道は2つのポリゴンで描画しているので2つともチェック
		result = HitCheck_Line_Triangle(startLinePos, endLinePos, leftTop, rightTop, leftBottom);
		result2 = HitCheck_Line_Triangle(startLinePos, endLinePos, rightBottom, leftBottom, rightTop);

		// 1つでもポリゴンとプレイヤーから延ばした線が当たっていたら処理終了
		// 当たった後も処理を続けると結果が書き変わってしまうため
		if (result.HitFlag || result2.HitFlag)
		{
			break;
		}
	}
}