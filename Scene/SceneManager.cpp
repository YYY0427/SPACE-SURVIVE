#include "SceneManager.h"
#include "Scene.h"

/// <summary>
/// シーンの切り替えを行う
/// </summary>
/// <param name="scene">切り替えたい次シーンのアドレス</param>
void SceneManager::ChangeScene(Scene* scene)
{
	//シーンスタックが空ではなかったら...
	if (!scene_.empty())
	{
		delete scene_.front();		// topはてっぺんの要素を返してる
		scene_.pop_front();			// 1個減る -> ふつうは0になる
	}
	scene_.push_front(scene);		// 1個増える
}

/// <summary>
/// シーンを上に積む(ポーズ)
/// Updateで実行されるのは上につまれたシーンのみ
/// </summary>
/// <param name="scene"></param>
void SceneManager::PushScene(Scene* scene)
{
	scene_.push_front(scene);		// 1個増える
}

/// <summary>
/// 一番上のシーンを削除する
/// </summary>
void SceneManager::PopScene()
{
	//ポップの結果、シーンが0にならないようにする
	if (scene_.size() > 1)
	{
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