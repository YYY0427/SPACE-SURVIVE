#include "SceneManager.h"
#include "Scene.h"

/// <summary>
/// シーンの切り替えを行う
/// </summary>
/// <param name="scene">切り替えたい次シーンのアドレス</param>
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

/// <summary>
/// シーンを上に積む(ポーズ)
/// Updateで実行されるのは上につまれたシーン
/// </summary>
/// <param name="scene"></param>
void SceneManager::PushScene(Scene* scene)
{
	// シーンの追加
	scene_.push_front(scene);		
}

/// <summary>
/// 一番上のシーンを削除する
/// </summary>
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

/// <summary>
/// 各シーンのUpdateを行う
/// </summary>
void SceneManager::Update(const InputState& input)
{
	scene_.front()->Update(input);
}

/// <summary>
/// 各シーンの描画
/// </summary>
void SceneManager::Draw()
{
	for (int i = static_cast<int>(scene_.size() - 1); i >= 0; --i)
	{
		scene_[i]->Draw();
	}
}