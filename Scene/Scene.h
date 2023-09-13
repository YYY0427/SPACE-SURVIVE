#pragma once
#include <DxLib.h>

// プロトタイプ宣言
class SceneManager;	// シーンマネージャー
class InputState;	// 入力ステート

//この時点ではSceneManagerとInputStateは
//ポインタもしくは参照としてしか扱えない。

/// <summary>
/// シーン基底クラス
/// </summary>
class Scene
{
public:
	Scene(SceneManager& manager);
	virtual ~Scene() {};

	// 更新
	virtual void Update(const InputState& input) = 0;

	// 描画
	virtual void Draw() = 0;

	// フェード
	void UpdateFade();		// フェードの更新
	void DrawFade();		// フェードの描画
	void StartFadeOut();	// フェードアウトの開始
	bool IsFadingIn() const;	// フェードイン中かどうか
	bool IsFadingOut() const;	// フェードアウト中かどうか
	bool IsFadeing() const;		// フェード中かどうか
	int GetFadeBright() const;	// フェードの明るさの取得
	void SetFadeConfig(int fadeSpeed, VECTOR fadeColor, int fadeBright);	// フェードの設定
protected:
	// シーンマネーシャーへの参照
	SceneManager& manager_; 

	// フェード
	int fadeColor_;		// フェードする色
	int fadeBright_;	// フェードの明るさ
	int fadeSpeed_;		// フェードする速度
	bool isFadeOut_;	// フェードアウトをおこなったかどうか
};

