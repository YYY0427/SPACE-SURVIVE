#pragma once
#include <DxLib.h>

// プロトタイプ宣言
class SceneManager;	

// シーンの基底クラス
class Scene
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="manager">シーンマネージャーの参照</param>
	Scene(SceneManager& manager);

	// デストラクタ
	virtual ~Scene();

	// 更新
	virtual void Update() = 0;

	// 描画
	virtual void Draw() = 0;

	// フェードの更新
	void UpdateFade();			

	// フェードの描画
	void DrawFade();

	// モザイクフェードの描画
	void DrawGaussFade();

	// フェードアウトの開始
	void StartFadeOut();

	/// <summary>
	/// フェードイン中かどうか
	/// </summary>
	/// <returns>true : フェードイン中、false : フェードインしていない</returns>
	bool IsFadingIn() const;

	/// <summary>
	/// フェードアウト中かどうか
	/// </summary>
	/// <returns>true : フェードアウト中、false : フェードアウトしていない</returns>
	bool IsFadingOut() const;	

	/// <summary>
	/// フェード中かどうか
	/// </summary>
	/// <returns>true : フェード中、false : フェードしてない</returns>
	bool IsFadeing() const;	

protected:
	// シーンマネーシャーの参照
	SceneManager& manager_; 

	// フェードの色
	unsigned int fadeColor_;		

	// フェードの明るさ
	// 0(フェードしていない)～255(真っ黒)
	int fadeBright_;	

	// フェードする速度
	// 正数の場合フェードアウト、負数の場合フェードイン
	int fadeSpeed_;		

	// フェードアウトをおこなったかどうか
	bool isFadeOut_;	

	int handle_;
};

