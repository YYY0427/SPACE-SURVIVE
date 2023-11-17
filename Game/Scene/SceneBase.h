#pragma once
#include <DxLib.h>

// プロトタイプ宣言
class SceneManager;	

// シーンの基底クラス
class SceneBase
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="manager">シーンマネージャーの参照</param>
	SceneBase(SceneManager& manager);

	// デストラクタ
	virtual ~SceneBase();

	// 更新
	virtual void Update() = 0;

	// 描画
	virtual void Draw() = 0;

	// フェードの更新
	void UpdateFade();			

	/// <summary>
	/// フェードの描画
	/// </summary>
	/// <param name="isPlay">true : フェードを行う, false: フェードを行わない</param>
	void DrawFade(bool isPlay);

	/// <summary>
	/// モザイクフェードの描画 
	/// </summary>
	/// <param name="isPlay">true : フェードを行う, false: フェードを行わない</param>
	void DrawGaussFade(bool isPlay);

	// フェードアウトの開始
	void StartFadeOut(int fadeBrightUpperLimitValue, int fadeSpeed = 8);

	// フェードインの開始
	void StartFadeIn(int fadeSpeed = 8);

	/// <summary>
	/// フェードイン中かどうか
	/// </summary>
	/// <returns>true : フェードイン中, false : フェードインしていない</returns>
	bool IsFadingIn() const;

	/// <summary>
	/// フェードアウト中かどうか
	/// </summary>
	/// <returns>true : フェードアウト中, false : フェードアウトしていない</returns>
	bool IsFadingOut() const;	

	/// <summary>
	/// フェード中かどうか
	/// </summary>
	/// <returns>true : フェード中, false : フェードしてない</returns>
	bool IsFadeing() const;	

	/// <summary>
	/// フェードアウトスタート後にフェードアウト中ではないか 
	/// </summary>
	/// <returns>true : フェードアウト処理をスタートしていてフェードアウト中ではない,
	///			false : フェードアウト処理をスタートしていない,　スタートしているがフェードアウト中</returns>
	bool IsStartFadeOutAfterFadingOut();

	// フェードの明るさの取得
	int GetFadeBright() const;

	/// <summary>
	/// フェードの明るさの設定
	/// </summary>
	/// <param name="fadeBright">設定したい明るさの値</param>
	void SetFadeBright(int fadeBright);

protected:
	// シーンマネーシャーの参照
	SceneManager& manager_;

private:
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

	// モザイクフェード用の描画グラフィック
	int gaussScreen_;

	// フェードの上限値
	// 0(フェードしない)～255(最後までフェードを行う)
	int fadeBrightUpperLimitValue_;
};

