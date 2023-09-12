#pragma once

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
	void UpdateFade();
	void DrawFade();
protected:
	// シーンマネーシャーへの参照
	SceneManager& manager_; 

	// フェード
	int fadeColor_;		// フェードする色
	int fadeBright_;	//	
	int fadeSpeed_;		// フェードする速度
};

