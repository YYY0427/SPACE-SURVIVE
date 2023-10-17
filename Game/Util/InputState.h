#pragma once

// ボタンの入力タイプ
enum class InputType
{
	DECISION,	// 決定
	BACK,		// 戻る
	PAUSE,		// ポーズ
	SLOW,		// スローモーション
	BOOST,		// ブースト
	CAMERA_INVERSION,	// カメラ反転
	UP,			// 上
	DOWN,		// 下
	RIGHT,		// 右
	LEFT,		// 左
	TOTAL_VALUE	// ボタンの入力タイプの合計値
};

// パッドのトリガーの左か右かまたはパッドのスティックの左か右か
enum class PadLR
{
	LEFT,	// トリガーまたはスティックの左
	RIGHT,	// トリガーまたはスティックの右
};

// パッドのスティックの入力タイプ
enum class PadStickInputType
{
	UP,				// 上
	DOWN,			// 下
	LEFT,			// 左
	RIGHT,			// 右
};

// 入力装置カテゴリ
enum class InputCategory
{
	KEYBORD,	// キーボード
	PAD,		// ゲームパッド
	MOUSE		// マウス
};

// 入力情報
struct InputInfo
{
	InputCategory cat;	// 入力装置カテゴリ
	int id;				// 入力ID(KEY_INPUT_〜,PAD_INPUT_〜,MOUSE_INPUT_〜)
};

// 入力状態を管理する
namespace InputState
{
	// 初期化
	void Init();

	// 入力情報の更新
	// 毎フレームUpdateを呼ばないと入力状態は更新されない
	void Update();

	/// <summary>
	/// ボタンが押された瞬間の入力情報の取得
	/// </summary>
	/// <param name="type">取得したい入力タイプ</param>
	/// <returns>true : 入力、false : 非入力</returns>
	bool IsTriggered(InputType type);

	/// <summary>
	/// ボタンが押されている間の入力情報の取得
	/// </summary>
	/// <param name="type">取得したい入力タイプ</param>
	/// <returns>true : 入力中、false : 非入力</returns>
	bool IsPressed(InputType type);

	/// <summary>
	/// パッドのトリガーの入力情報の取得
	/// </summary>
	/// <param name="type">取得したい入力タイプ</param>
	/// <returns>true : 入力中、false : 非入力</returns>
	bool IsPadTrigger(PadLR type);

	/// <summary>
	/// パッドのスティックの入力情報を取得
	/// </summary>
	/// <param name="stick">スティックの右か左か</param>
	/// <param name="input">スティックのどの入力タイプか</param>
	/// <returns>スティックの傾きぐわい(0のときは入力なし)　傾いている角度が大きいほど大きい数字が返ってくる</returns>
	int IsPadStick(PadLR stic, PadStickInputType type);
};