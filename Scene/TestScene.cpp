#include "TestScene.h"
#include "SceneManager.h"
#include "DebugScene.h"
#include "PauseScene.h"
#include "../Util/DrawFunctions.h"
#include "../Util/Effekseer3DEffectManager.h"
#include "../Util/SoundManager.h"
#include "../Util/InputState.h"
#include "../Util/DataReaderFromUnity.h"
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

// コンストラクタ
TestScene::TestScene(SceneManager& manager) :
	Scene(manager),
	updateFunc_(&TestScene::NormalUpdate)
{
	// オブジェクトの配置データの読み込み
	pDataReader_ = std::make_shared<DataReaderFromUnity>();
	pDataReader_->LoadUnityGameObjectData();
	auto playerData = pDataReader_->GetData().find("Player")->second.front();
	auto rockData = pDataReader_->GetData().find("Rock")->second;
	auto meteorData = pDataReader_->GetData().find("Meteor")->second;
	auto sunData = pDataReader_->GetData().find("Sun")->second;
	auto earthData = pDataReader_->GetData().find("Earth")->second;
	auto roadData = pDataReader_->GetData().find("Stage")->second;

	pImg3DManager_ = std::make_shared<Image3DManager>(roadData);
	pPlayer_ = std::make_shared<Player>(playerData);
	pRockManager_ = std::make_shared<RockManager>(rockData, meteorData, pPlayer_);
	pPlanetManager_ = std::make_shared<PlanetManager>(sunData, earthData);
	pCamera_ = std::make_shared<Camera>(pPlayer_);
	pSkyDome_ = std::make_shared<SkyDome>(pPlayer_);

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
	// 現在のシーンのテキスト表示
	DrawString(0, 0, "TestScene", 0xffffff, true);

	// 各クラスの描画
	pSkyDome_->Draw();
	pImg3DManager_->Draw();
//	GroundLineDraw();
	pRockManager_->Draw();
	pPlanetManager_->Draw();
	pPlayer_->Draw();

	for (auto& road : pImg3DManager_->GetRoads())
	{
		VECTOR leftTop = road->GetVertex()[0].pos;
		VECTOR rightTop = road->GetVertex()[1].pos;
		VECTOR leftBottom = road->GetVertex()[2].pos;
		VECTOR rightBottom = road->GetVertex()[3].pos;

		DrawSphere3D(leftBottom, 32, 8, 0xff0000, 0xff0000, 0xff0000);
	}
	/*for (auto& road : pImg3DManager_->GetRoads())
	{
		float up = road->GetPos().z + road->GetImgHeight();
		float down = road->GetPos().z - road->GetImgHeight();
		float right = road->GetPos().x + road->GetImgWidth();
		float left = road->GetPos().x - road->GetImgWidth();
		DrawFormatString(50, 200, 0xffffff, "%f, %f, %f, %f", up, down, right, left);
	}*/

	// フェードの描画
	DrawFade(true);

	// モザイクフェードの描画
	DrawGaussFade(true);
}

// 通常の更新
void TestScene::NormalUpdate()
{
	// 各クラスの更新
	pSkyDome_->Update();
	pCamera_->Update();
	pPlayer_->Update();
	pRockManager_->Update();
	pPlanetManager_->Update();

	// 道の上にいなかったらプレイヤーが落下する処理
	bool isFall = true;
	for (auto& road : pImg3DManager_->GetRoads())
	{
		VECTOR leftTop = road->GetVertex()[0].pos;
		VECTOR rightTop = road->GetVertex()[1].pos;
		VECTOR leftBottom = road->GetVertex()[2].pos;
		VECTOR rightBottom = road->GetVertex()[3].pos;

		VECTOR top = VAdd(leftTop, rightTop);
		VECTOR bottm = VAdd(leftBottom, rightBottom);
		VECTOR right = VAdd(rightTop, rightBottom);
		VECTOR left = VAdd(leftTop, leftBottom);


		VECTOR pos = pPlayer_->GetPos();
		if (leftTop.x < pos.x && rightTop.x > pos.x && leftBottom.x < pos.x && rightBottom.x > pos.x && 
			leftTop.z > pos.z && rightTop.z > pos.z && leftBottom.z < pos.z && rightBottom.z < pos.z)
		{
			isFall = false;
		}
		if (!isFall)
		{
			break;
		}

		//float up = road->GetPos().z + road->GetImgHeight();
		//float down = road->GetPos().z - road->GetImgHeight();
		//float right = road->GetPos().x + road->GetImgWidth();
		//float left = road->GetPos().x - road->GetImgWidth();

		//if (pPlayer_->GetPos().x < right && pPlayer_->GetPos().x > left &&
		//	pPlayer_->GetPos().z < up && pPlayer_->GetPos().z > down &&
		//	pPlayer_->GetPos().y > road->GetPos().y)
		//{
		//	// 道の上にいる
		//	isFall = false;
		//	break;
		//}
	}
	if (isFall)
	{
		// 道の上にいなかったので落下
		pPlayer_->Fall(10.0f);
	}

	// 敵とぶつかったらゲームオーバー
	for (auto& rocks : pRockManager_->GetRocks())
	{
		MV1_COLL_RESULT_POLY_DIM result = MV1CollCheck_Sphere(rocks->GetModelHandle(), -1, pPlayer_->GetPos(), pPlayer_->GetCollsionRadius());
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