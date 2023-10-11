#pragma once
#include <deque>

// プロトタイプ宣言
class Scene;

// シーンの管理クラス
class SceneManager
{
public:
	/// <summary>
	/// 全てのシーンの削除とシーンの切り替え
	/// 主にポーズからのシーンの切り替えを行うときに使う
	/// </summary>
	/// <param name="scene">切り替えたいシーンのアドレス</param>
	void PopAllSceneAndChangeScene(Scene* scene);

	/// <summary>
	/// シーンの切り替え
	/// </summary>
	/// <param name="scene">切り替えたいシーンのアドレス</param>
	void ChangeScene(Scene* scene);

	/// <summary>
	/// 現在のシーンの上にシーンを積む(ポーズ)
	/// Updateで実行されるのは上につまれたシーン
	/// </summary>
	/// <param name="scene">上に積みたいシーンのアドレス</param>
	void PushScene(Scene* scene);

	// 一番上のシーンを削除
	// ポーズ画面を消すときに使う
	void PopScene();

	/// <summary>
	/// ゲームを終了するかどうかの取得
	/// main.cppでwhile分を抜けるために作成
	/// </summary>
	/// <returns>true : ゲーム終了, false : 終了しない</returns>
	bool GetIsGameEnd() const;

	/// <summary>
	/// ゲームを終了するかどうかの設定
	/// main.cppでwhile分を抜けるために作成
	/// </summary>
	/// <param name="isGameEnd">true : ゲーム終了, false : 終了しない</param>
	void SetIsGameEnd(bool isGameEnd);

	// シーンの更新
	// 一番上のシーンだけ更新
	void Update();

	// 各シーンの描画
	void Draw();

private:
	// シーン
	// 今実行中のシーンを切り替えていきたいので	
	// 参照ではなくポインタとして宣言
	std::deque<Scene*> scene_;

	// ゲームを終了するかどうか
	bool isGameEnd_;
};

