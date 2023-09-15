#include "InputState.h"
#include <DxLib.h>
#include <map>
#include <vector>
#include <string>

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
	/// <param name="stickType">スティックの右か左か</param>
	/// <returns>スティックのどの入力タイプか</returns>
	XInputTypeStick IsXInputStick(XInputType stickType)
	{
		// パッドの情報の取得
		XINPUT_STATE  padState;
		GetJoypadXInputState(DX_INPUT_PAD1, &padState);

		// 右スティックか左スティックか
		float x, y;
		if (stickType == XInputType::LEFT)
		{
			// padStateから取得した値を-1.0~1.0に変換
			x = static_cast<float>(padState.ThumbLX / 32767.0f);
			y = static_cast<float>(padState.ThumbLY / 32767.0f);
		}
		else
		{
			// padStateから取得した値を-1.0~1.0に変換
			x = static_cast<float>(padState.ThumbRX / 32767.0f);
			y = static_cast<float>(padState.ThumbRY / 32767.0f);
		}

		// 変換した値の割合によってreturnする
		if (x < 0.8f && x > 0.3f)
		{
			// 右にちょっと動く
			return XInputTypeStick::LITTLE_RIGHT;
		}
		else if (x > 0.9f)
		{
			// 右に大きく動く
			return XInputTypeStick::RIGHT;
		}
		else if (x < -0.3f && x > -0.8f)
		{
			// 左に少し動く
			return XInputTypeStick::LITTLE_LEFT;
		}
		else if (x < -0.9f)
		{
			// 左に大きく動く
			return XInputTypeStick::LEFT;
		}

		if (y < 0.8f && y > 0.3f)
		{
			// 上にちょっと動く
			return XInputTypeStick::LITTLE_UP;
		}
		else if (y > 0.9f)
		{
			// 上に大きく動く
			return XInputTypeStick::UP;
		}
		else if (y < -0.3f && y > -0.8f)
		{
			// 下に少し動く
			return XInputTypeStick::LITTLE_DOWN;
		}
		else if (y < -0.9f)
		{
			// 下に大きく動く
			return XInputTypeStick::DOWN;
		}
		// 動いてない
		return XInputTypeStick::NONE;
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