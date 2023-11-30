#include "SceneManager.h"
#include "SceneBase.h"
#include "../common.h"
#include "../Util/Effekseer3DEffectManager.h"

// 全てのシーンの削除とシーンの切り替え
void SceneManager::PopAllSceneAndChangeScene(SceneBase* scene)
{
	// すべてのシーンの削除
	for (auto& scene : scene_)
	{
		// シーンの削除
		delete scene;
	}
	// すべて配列を削除
	scene_.clear();

	// 次のシーンの追加
	PushScene(scene);
}

// シーンの切り替え
void SceneManager::ChangeScene(SceneBase* scene)
{
	// シーンスタックが空ではなかったら
	if (!scene_.empty())
	{
		// 現在のシーンの削除
		delete scene_.front();		

		// 削除したシーンの配列を削除
		scene_.pop_front();			
	}
	// 次のシーンの追加
	PushScene(scene);
}

// 現在のシーンの上にシーンを積む(ポーズ)
void SceneManager::PushScene(SceneBase* scene)
{
	// シーンの追加
	scene_.push_front(scene);
}

// 一番上のシーンを削除する
void SceneManager::PopScene()
{
	// シーンが0にならないようにする
	if (scene_.size() > 1)
	{
		// 現在のシーンの削除
		delete scene_.front();

		// 削除したシーンの配列を削除
		scene_.pop_front();
	}
}

// ゲームを終了するかどうかの取得
bool SceneManager::GetIsGameEnd() const
{
	return isGameEnd_;
}

// ゲームを終了するかどうかの設定
void SceneManager::SetIsGameEnd(bool isGameEnd)
{
	isGameEnd_ = isGameEnd;
}

// シーンの更新
void SceneManager::Update()
{
	LONGLONG start = GetNowHiPerformanceCount();

	if (static_cast<int>(scene_.size()) == 1)
	{
		Effekseer3DEffectManager::GetInstance().Update();
	}
	scene_.front()->Update();

	updateTime_ = GetNowHiPerformanceCount() - start;
}

// 各シーンの描画
void SceneManager::Draw()
{
	LONGLONG start = GetNowHiPerformanceCount();

	for (int i = static_cast<int>(scene_.size() - 1); 0 <= i; i--)
	{
		scene_[i]->Draw();
	}
	if (static_cast<int>(scene_.size()) == 1)
	{
		Effekseer3DEffectManager::GetInstance().Draw();
	}

	drawTime_ = GetNowHiPerformanceCount() - start;

	float rate = static_cast<float>(updateTime_ + drawTime_) / static_cast<float>((1000 / common::fps) * 1000);
	int width = static_cast<int>(common::screen_width * rate);
	DrawBox(0, common::screen_height - 16, width, common::screen_height, 0xff0000, true);

	rate = static_cast<float>(updateTime_) / static_cast<float>((1000 / common::fps) * 1000);
	width = static_cast<int>(common::screen_width * rate);
	DrawBox(0, common::screen_height - 16, width, common::screen_height, 0x0000ff, true);
}