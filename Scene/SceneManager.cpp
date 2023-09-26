#include "SceneManager.h"
#include "Scene.h"

// 全てのシーンの削除とシーンの切り替え
void SceneManager::PopAllSceneAndChangeScene(Scene* scene)
{
	// すべてのシーンの削除
	for (auto& scene : scene_)
	{
		delete scene_.front();
		scene_.pop_front(); 
	}

	// 次のシーンの追加
	scene_.push_front(scene);
}

// シーンの切り替え
void SceneManager::ChangeScene(Scene* scene)
{
	// シーンスタックが空ではなかったら
	if (!scene_.empty())
	{
		// 現在のシーンの削除
		delete scene_.front();		
		scene_.pop_front();			
	}
	// 次のシーンの追加
	scene_.push_front(scene);		
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
		// シーンの削除
		delete scene_.front();
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
}

// 各シーンの描画
void SceneManager::Draw()
{
	for (int i = static_cast<int>(scene_.size() - 1); 0 <= i; i--)
	{
		scene_[i]->Draw();
	}
}