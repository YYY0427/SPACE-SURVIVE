#pragma once
#include <deque>

class Scene;

/// <summary>
/// シーン監視クラス
/// </summary>
class SceneManager
{
public:
	// シーンの切り替え
	void ChangeScene(Scene* scene);

	// シーンを上に積む(ポーズ)
	void PushScene(Scene* scene);

	/// 一番上のシーンを削除
	void PopScene();

	// 各シーンの更新
	void Update();

	// 各シーンの描画
	void Draw();

private:
	// 今実行中のシーンを切り替えていきたいので	
	// 参照ではなくポインタとして宣言
	std::deque<Scene*> scene_;
};

