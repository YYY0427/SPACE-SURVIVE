#include "InputState.h"
#include <DxLib.h>
#include <map>
#include <vector>
#include <string>
#include <cmath>

namespace
{
	// 仮想入力情報と実際の入力のテーブル
	std::map<InputType, std::vector<InputInfo>> inputMapTable_;

	// 現在の入力情報(押してるか押してないか)
	std::vector<bool> currentInput_;

	// 直前の入力情報(直前押してるか押してないか)
	std::vector<bool> lastInput_;
}

namespace InputState
{
	/// <summary>
	/// 初期化
	/// </summary>
	void Init()
	{
		// ボタンの入力タイプをどのボタンにするかの設定
		inputMapTable_[InputType::DECISION] = { {InputCategory::PAD,  XINPUT_BUTTON_A } };			// Aボタン
		inputMapTable_[InputType::BACK] = { {InputCategory::PAD, XINPUT_BUTTON_B} };				// Bボタン
		inputMapTable_[InputType::PAUSE] = { {InputCategory::PAD, XINPUT_BUTTON_START } };			// スタートボタン
		inputMapTable_[InputType::RIZE] = { {InputCategory::PAD, XINPUT_BUTTON_A } };				// Aボタン
		inputMapTable_[InputType::UP] = { {InputCategory::PAD, XINPUT_BUTTON_DPAD_UP } };			// ↑
		inputMapTable_[InputType::DOWN] = { {InputCategory::PAD, XINPUT_BUTTON_DPAD_DOWN } };		// ↓
		inputMapTable_[InputType::RIGHT] = { {InputCategory::PAD, XINPUT_BUTTON_DPAD_RIGHT } };		// →
		inputMapTable_[InputType::LEFT] = { {InputCategory::PAD, XINPUT_BUTTON_DPAD_LEFT } };		// ←

		// 設定したボタンの数によって配列の数を変更
		currentInput_.resize(static_cast<int>(InputType::NUM));
		lastInput_.resize(static_cast<int>(InputType::NUM));
	}

	/// <summary>
	/// 入力情報の更新
	/// (注意)毎フレームUpdateを呼ばないと入力状態は更新されない
	/// </summary>
	void Update()
	{
		// 直前の入力情報を記憶しておく
		lastInput_ = currentInput_;

		// パッドの情報の取得
		XINPUT_STATE  padState;
		GetJoypadXInputState(DX_INPUT_PAD1, &padState);

		// キー情報の取得
		char keystate[256];
		GetHitKeyStateAll(keystate);

		// マウス情報の取得
		int mouseState = GetMouseInput();

		// マップの全情報をループ
		for (const auto& keymap : inputMapTable_)
		{
			// 入力情報配列をループ
			for (const auto& input : keymap.second)
			{
				// 入力情報からどのボタンが入力されているか確認
				if (input.cat == InputCategory::KEYBORD)
				{
					currentInput_[static_cast<int>(keymap.first)] = keystate[input.id];
				}
				else if (input.cat == InputCategory::PAD)
				{
					currentInput_[static_cast<int>(keymap.first)] = padState.Buttons[input.id];
				}
				else if (input.cat == InputCategory::MOUSE)
				{
					currentInput_[static_cast<int>(keymap.first)] = mouseState & input.id;
				}
				// 3つの入力のうちどれかがtrueだったら入力されてるからbreak
				if (currentInput_[static_cast<int>(keymap.first)])
				{
					break;
				}
			}
		}
	}

	/// <summary>
	/// ボタンが押された瞬間の入力情報の取得
	/// </summary>
	/// <param name="type">取得したい入力タイプ</param>
	/// <returns>true : 入力、false : 非入力</returns>
	bool IsPressed(InputType type)
	{
		return currentInput_[static_cast<int>(type)];
	}

	/// <summary>
	/// ボタンが押されている間の入力情報の取得
	/// </summary>
	/// <param name="type">取得したい入力タイプ</param>
	/// <returns>true : 入力中、false : 非入力</returns>
	bool IsTriggered(InputType type)
	{
		return IsPressed(type) && !lastInput_[static_cast<int>(type)];
	}

	/// <summary>
	/// パッドのスティックの入力情報を取得
	/// </summary>
	/// <param name="stick">スティックの右か左か</param>
	/// <param name="input">スティックのどの入力タイプか</param>
	/// <returns>スティックの傾きぐわい(0のときは入力なし)　傾いている角度が大きいほど大きい数字が返ってくる</returns>
	int IsXInputStick(XInputType stick, XInputTypeStick input)
	{
		// パッドの情報の取得
		XINPUT_STATE  padState;
		GetJoypadXInputState(DX_INPUT_PAD1, &padState);

		// 右スティックか左スティックか
		float x, y;
		if (stick == XInputType::LEFT)
		{
			// padStateから取得した値を-1.0~1.0に変換
			x = static_cast<float>(padState.ThumbLX / 32767.0f * 10);
			y = static_cast<float>(padState.ThumbLY / 32767.0f * 10);
		}
		else
		{
			// padStateから取得した値を-1.0~1.0に変換
			x = static_cast<float>(padState.ThumbRX / 32767.0f * 10);
			y = static_cast<float>(padState.ThumbRY / 32767.0f * 10);
		}
		if (input == XInputTypeStick::LEFT && x < -0.1f)
		{
			return abs(static_cast<int>(x));
		}
		if (input == XInputTypeStick::RIGHT && x > 0.1f)
		{
			return abs(static_cast<int>(x));
		}
		if (input == XInputTypeStick::UP && y < -0.1f)
		{
			return abs(static_cast<int>(y));
		}
		if (input == XInputTypeStick::DOWN && y > 0.1f)
		{
			return abs(static_cast<int>(y));
		}
		// 入力なし
		return 0;
	}

	/// <summary>
	/// パッドのトリガーの入力情報の取得
	/// </summary>
	/// <param name="type">取得したい入力タイプ</param>
	/// <returns>true : 入力中、false : 非入力</returns>
	bool IsXInputTrigger(XInputType type)
	{
		// パッドの情報の取得
		XINPUT_STATE  padState;
		GetJoypadXInputState(DX_INPUT_PAD1, &padState);

		// トリガーの左か右か
		if (type == XInputType::LEFT)
		{
			// トリガーが半分以上押されていたらtrue
			return 	padState.LeftTrigger > padState.LeftTrigger / 2;
		}
		else
		{
			// トリガーが半分以上押されていたらtrue
			return 	padState.RightTrigger > padState.RightTrigger / 2;
		}
	}
}