#pragma once
#include <map>
#include <vector>
#include <string>

/// <summary>
/// ボタンの入力タイプ
/// </summary>
enum class InputType
{
	DECISION,	// 決定
	BACK,		// 戻る
	PAUSE,		// ポーズボタン
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
class InputState
{
public:
	// コンストラクタ
	InputState();

	// 入力情報の更新
	void Update();

	// ボタンが押された瞬間の入力情報の取得
	bool IsTriggered(InputType type) const;

	// ボタンが押されている間の入力情報の取得
	bool IsPressed(InputType type) const;

	// パッドのトリガーの入力情報の取得
	bool IsXInputTrigger(XInputType type) const;

	// パッドのスティックの入力情報を取得
	XInputTypeStick IsXInputStick(XInputType stic) const;

private:
	// 仮想入力情報と実際の入力のテーブル
	std::map<InputType, std::vector<InputInfo>> inputMapTable_;

	// 現在の入力情報(押してるか押してないか)
	std::vector<bool> currentInput_;	

	// 直前の入力情報(直前押してるか押してないか)
	std::vector<bool> lastInput_;		
};