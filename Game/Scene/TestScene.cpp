#include "TestScene.h"
#include "TitleScene.h"
#include "PauseScene.h"
#include "SceneManager.h"
#include "../common.h"
#include "../Util/InputState.h"
#include "../Util/Effekseer3DEffectManager.h"
#include "../Util/SoundManager.h"
#include "../Util/Debug.h"
#include "../Camera.h"
#include "../UI/Warning.h"
#include "../UI/HpBar.h"
#include <DxLib.h>

// コンストラクタ
TestScene::TestScene(SceneManager& manager) :
	SceneBase(manager)
{
	pWarning_ = std::make_shared<Warning>(600);

	pCamera_ = std::make_shared<Camera>();

	auto& effectManager = Effekseer3DEffectManager::GetInstance();
	effectManager.PlayEffect(laserEffectHandle_, EffectID::test, {0, 0, 0}, {30, 30, 30}, 1.0, {0.0, DX_PI_F, 0.0});

//	pHpBar_ = std::make_shared<HpBar>(50);
}

// デストラクタ
TestScene::~TestScene()
{
	// 処理なし
}

// 更新
void TestScene::Update()
{
	// 各クラスの更新
//	pHpBar_->Update(30);
//	pWarning_->Update();

	pCamera_->Update();

	auto& effectManager = Effekseer3DEffectManager::GetInstance();
	if (InputState::IsTriggered(InputType::UP))
	{
		effectManager.SetDynamicEffectParam(laserEffectHandle_, 0, 1.0f);

	}
	float param = effectManager.GetDynamicEffectParam(laserEffectHandle_, 0);
	Debug::Log("レーザーのなんかの値", param);

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

// 描画
void TestScene::Draw()
{
	// 現在のシーンのテキスト表示
	Debug::Log("TestScene");

	// 各クラスの描画
//	pWarning_->Draw();
//	pHpBar_->Draw(50, 20, 50);

	// フェードの描画
	DrawFade(true);

	// モザイクフェードの描画
	DrawGaussFade(true);
}