#include "TestScene.h"
#include "SceneManager.h"
#include "DebugScene.h"
#include "PauseScene.h"
#include "../Util/Effekseer3DEffectManager.h"
#include "../Util/SoundManager.h"
#include "../Util/InputState.h"
#include "../Camera.h"
#include "../Player.h"
#include "../SkyDome.h"
#include "../EnemyManager.h"
#include "../Enemy.h"
#include "../Game.h"

/// <summary>
/// コンストラクタ
/// </summary>
TestScene::TestScene(SceneManager& manager) :
	Scene(manager),
	updateFunc_(&TestScene::NormalUpdate)
{
	pPlayer_ = std::make_shared<Player>();
	pCamera_ = std::make_shared<Camera>(*pPlayer_);
	pSkyDome_ = std::make_shared<SkyDome>();
	pEnemyManager_ = std::make_shared<EnemyManager>(*pPlayer_);
	pPlayer_->SetCamera(pCamera_);
}

/// <summary>
///  デストラクタ
/// </summary>
TestScene::~TestScene()
{
}

/// <summary>
/// 更新
/// </summary>
void TestScene::Update()
{
	(this->*updateFunc_)();
}

/// <summary>
/// 描画
/// </summary>
void TestScene::Draw()
{
	DrawBox(0, 0, Game::screen_width, Game::screen_height, 0xffffff, true);
	DrawString(0, 0, "TestScene", 0xffffff, true);

	VECTOR pos1;
	VECTOR pos2;
	float lineAreaSize = 10000.0f;
	int lineNum = 50;

	pos1 = VGet(-lineAreaSize / 2.0f, 0.0f, -lineAreaSize / 2.0f);
	pos2 = VGet(-lineAreaSize / 2.0f, 0.0f, lineAreaSize / 2.0f);
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
//	pSkyDome_->Draw();
	pEnemyManager_->Draw();
	pPlayer_->Draw();

	// フェードの描画
	DrawFade();
}

/// <summary>
/// 通常の更新
/// </summary>
void TestScene::NormalUpdate()
{
	// フェードアウトが終わり次第シーン遷移
	/*if (isFadeOut_ && !IsFadingOut())
	{
		manager_.ChangeScene(new DebugScene(manager_));
		return;
	}*/

	pCamera_->Update();
	pPlayer_->Update();
//	pSkyDome_->Update();
	pEnemyManager_->Update();

	// 敵とぶつかったらゲームオーバー
	for (auto& enemies : pEnemyManager_->GetEnemies())
	{
		MV1_COLL_RESULT_POLY_DIM result = MV1CollCheck_Sphere(enemies->GetModelHandle(), -1, pPlayer_->GetPos(), pPlayer_->GetCollsionRadius());
		if (result.HitNum > 0)
		{
			updateFunc_ = &TestScene::GameOverUpdate;
		}
	}

	if (InputState::IsTriggered(InputType::PAUSE))
	{
		manager_.PushScene(new PauseScene(manager_));
	}

	// 戻るボタンが押されてフェードインしてなかったらフェードアウト開始
	//if (InputState::IsTriggered(InputType::BACK) && !IsFadingIn())
	//{
	//	StartFadeOut();

	//	// フェードの設定の変更
	//	SetFadeConfig(3, VGet(255, 255, 255), GetFadeBright());

	//	isFadeOut_ = true;
	//}
	
	// フェードの更新
	UpdateFade();
}

/// <summary>
/// ゲームオーバー時の更新
/// </summary>
void TestScene::GameOverUpdate()
{
	// フェードアウトが終わり次第シーン遷移
	if (isFadeOut_ && !IsFadingOut())
	{
		manager_.ChangeScene(new DebugScene(manager_));
		return;
	}

	// カメラの更新
	pCamera_->Update();

	// プレイヤーのゲームオーバー時の更新が終了かつフェードインしてなかったらフェードアウト開始
	if (pPlayer_->GameOverUpdate() && !IsFadingIn())
	{
		// フェードアウトの開始
		StartFadeOut();

		// フェードの設定の変更
		fadeColor_ = 0x000000;
		fadeSpeed_ = 3;
	//	SetFadeConfig(3, VGet(255, 255, 255), GetFadeBright());

		isFadeOut_ = true;
	}
	// フェードの更新
	UpdateFade();
}