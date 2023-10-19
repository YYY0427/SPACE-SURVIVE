#include "SceneManager.h"
#include "Scene.h"
#include "../Util/Effekseer3DEffectManager.h"

// 全てのシーンの削除とシーンの切り替え
void SceneManager::PopAllSceneAndChangeScene(Scene* scene)
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
void SceneManager::ChangeScene(Scene* scene)
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
void SceneManager::PushScene(Scene* scene)
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
	scene_.front()->Update();

	if (static_cast<int>(scene_.size()) == 1)
	{
		Effekseer3DEffectManager::GetInstance().Update();
	}
}

// 各シーンの描画
void SceneManager::Draw()
{
	for (int i = static_cast<int>(scene_.size() - 1); 0 <= i; i--)
	{
		scene_[i]->Draw();
	}
	if (static_cast<int>(scene_.size()) == 1)
	{
		Effekseer3DEffectManager::GetInstance().Draw();
	}
}