#pragma once


/// <summary>
/// ボタンの入力タイプ
/// </summary>
enum class InputType
{
	DECISION,	// 決定
	BACK,		// 戻る
	PAUSE,		// ポーズボタン
	SLOW,		// 上昇
	FALL,		// 下降
	BOOST,		// ブースト
	UP,			// 上
	DOWN,		// 下
	RIGHT,		// 右
	LEFT,		// 左
	NUM			// 最大入力インデックス
};

/// <summary>
/// パッドのトリガーまたはスティックの左か右か
/// </summary>
enum class XInputType
{
	LEFT,	// トリガーまたはスティックの左
	RIGHT,	// トリガーまたはスティックの右
};

/// <summary>
/// パッドのスティックの入力タイプ
/// </summary>
enum class XInputTypeStick
{
	NONE,			// 入力なし
	UP,				// 上
	DOWN,			// 下
	LEFT,			// 左
	RIGHT,			// 右
	LITTLE_UP,		// 少し上
	LITTLE_DOWN,	// 少し下
	LITTLE_LEFT,	// 少し左
	LITTLE_RIGHT,	// 少し右
	NUM				// 最大入力インデックス
};

/// <summary>
/// 入力装置カテゴリ
/// </summary>
enum class InputCategory
{
	KEYBORD,	// キーボード
	PAD,		// ゲームパッド
	MOUSE		// マウス
};

/// <summary>
/// 入力情報
/// </summary>
struct InputInfo
{
	InputCategory cat;	// 入力装置カテゴリ
	int id;				// 入力ID(KEY_INPUT_～,PAD_INPUT_～,MOUSE_INPUT_～)
};

/// <summary>
/// 入力状態を管理する
/// </summary>
namespace InputState
{
	// 初期化
	void Init();

	// 入力情報の更新
	void Update();

	// ボタンが押された瞬間の入力情報の取得
	bool IsTriggered(InputType type);

	// ボタンが押されている間の入力情報の取得
	bool IsPressed(InputType type);

	// パッドのトリガーの入力情報の取得
	bool IsXInputTrigger(XInputType type);

	// パッドのスティックの入力情報を取得
	int IsXInputStick(XInputType stic, XInputTypeStick input);
};