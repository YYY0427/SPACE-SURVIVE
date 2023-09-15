#include "InputState.h"
#include <DxLib.h>

InputState::InputState()
{
	defaultMapTable_[InputType::DECISION] = { {InputCategory::pad,  XINPUT_BUTTON_A }};			// Aボタン

	defaultMapTable_[InputType::BACK] = { {InputCategory::pad, XINPUT_BUTTON_B} };				// Bボタン

	defaultMapTable_[InputType::PAUSE] = {{InputCategory::pad, XINPUT_BUTTON_START } };			// スタートボタン

	defaultMapTable_[InputType::change] = { {InputCategory::pad, XINPUT_BUTTON_X } };			// Xショルダー

	defaultMapTable_[InputType::UP] = { {InputCategory::pad, XINPUT_BUTTON_DPAD_UP } };			// ↑

	defaultMapTable_[InputType::DOWN] = { {InputCategory::pad, XINPUT_BUTTON_DPAD_DOWN } };		// ↓

	defaultMapTable_[InputType::RIGHT] = {{InputCategory::pad, XINPUT_BUTTON_DPAD_RIGHT } };	// →
	 
	defaultMapTable_[InputType::LEFT] = { {InputCategory::pad, XINPUT_BUTTON_DPAD_LEFT } };		// ←

	inputMapTable_ = defaultMapTable_;

	LoadKeyInfo();

	//一時マップテーブルにコピー
	tempMapTable_ = inputMapTable_;

	//入力タイプの名前のテーブルを作る
	inputNameTable_[InputType::DECISION] = "決定";
	inputNameTable_[InputType::BACK] = "戻る";
	inputNameTable_[InputType::PAUSE] = "ポーズ";
	inputNameTable_[InputType::change] = "change";

	currentInput_.resize(static_cast<int>(InputType::NUM));
	lastInput_.resize(static_cast<int>(InputType::NUM));
}

void InputState::Update()
{
	lastInput_ = currentInput_;	//直前の入力情報を記憶しておく

	XINPUT_STATE  padState;
	GetJoypadXInputState(DX_INPUT_PAD1, &padState);

	char keystate[256];
	GetHitKeyStateAll(keystate); //全キー情報取得

	int mouseState = GetMouseInput();

	//マップの全情報をループする
	for (const auto& keymap : inputMapTable_)
	{
		//入力情報配列をループする
		for (const auto& input : keymap.second)
		{
			//このinputの中身は、keybd,KEY_INPUT_RETURNなどのセット(InputInfo)が入っている
			//keymap.secondは、この入力情報セットInputInfoが入っている
			//keymap.firstは、対応するゲーム入力名の"InputType::next"などが入っている
			if (input.cat == InputCategory::keybd)
			{
				currentInput_[static_cast<int>(keymap.first)] = keystate[input.id];
			}
			else if (input.cat == InputCategory::pad)
			{
				currentInput_[static_cast<int>(keymap.first)] = padState.Buttons[input.id];
			}
			else if (input.cat == InputCategory::mouse)
			{
				currentInput_[static_cast<int>(keymap.first)] = mouseState & input.id;
			}
			//3つの入力のうちどれかがtrueだったらもう「入力されてる」
			//とみなして、breakする。
			if (currentInput_[static_cast<int>(keymap.first)])
			{
				break;
			}
		}
	}
}

void InputState::RewriteInputInfo(InputType type, InputCategory cat, int id)
{
	//入力種別(割り当て先)がなければ、無効なので無視します
	if (tempMapTable_.count(type) == 0)
	{
		return;
	}
	bool isRewrite = false;
	for (auto& inputInfo : tempMapTable_[type])
	{
		//カテゴリがヒットしたら
		if (inputInfo.cat == cat)
		{
			//IDを上書きする
			inputInfo.id = id;
			isRewrite = true;
			break;
		}
	}
	//もしカテゴリが存在しなかったら、ここで追加しておく
	if (!isRewrite)
	{
		tempMapTable_[type].push_back({ cat, id });
	}
}

void InputState::CommitChangedInputInfo()
{
	inputMapTable_ = tempMapTable_;
}

void InputState::RollbackChangedInputInfo()
{
	tempMapTable_ = inputMapTable_;
}

void InputState::ResetInputInfo()
{
	inputMapTable_ = defaultMapTable_;
	tempMapTable_ = defaultMapTable_;
}

void InputState::SaveKeyInfo() const
{
	FILE* fp = nullptr;

	auto err = fopen_s(&fp, "key.info", "wb");
	if (fp == nullptr)
	{
		return;
	}

	//仮想キータイプの数を書き込む
	int keytypeNum = static_cast<int>(inputMapTable_.size());
	fwrite(&keytypeNum, sizeof(keytypeNum), 1, fp);

	//仮想キータイプ(next, prevなど)のループ
	for (const auto& key : inputMapTable_)
	{
		int keytype = static_cast<int>(key.first);

		//仮想キー番号
		fwrite(&keytype, sizeof(keytype), 1, fp);
		int dataSize = static_cast<int>(key.second.size());

		//いくつ実入力データがあるのか
		fwrite(&dataSize, sizeof(dataSize), 1, fp);

		//vector型のdata()は先頭のアドレスを返す
		//一気にその入力のデータを書き込む
		fwrite(key.second.data(), dataSize * sizeof(InputInfo), 1, fp);
	}
	fclose(fp);
}

void InputState::LoadKeyInfo()
{
	int handle = FileRead_open("key.info");

	if (handle == 0)
	{
		return;
	}

	int keyTypeNum = 0;

	FileRead_read(&keyTypeNum, sizeof(keyTypeNum), handle);
	inputMapTable_.clear();

	for (int i = 0; i < keyTypeNum; i++)
	{
		int  inputType;
		FileRead_read(&inputType, sizeof(inputType), handle);

		int dataSize = 0;
		FileRead_read(&dataSize, sizeof(dataSize), handle);
		std::vector<InputInfo> inputInfoes(dataSize);
		FileRead_read(inputInfoes.data(), sizeof(InputInfo) * dataSize, handle);
		inputMapTable_[static_cast<InputType>(inputType)] = inputInfoes;
	}
	tempMapTable_ = inputMapTable_;
	FileRead_close(handle);
}

bool InputState::IsPressed(InputType type) const
{
	return currentInput_[static_cast<int>(type)];
}

bool InputState::IsTriggered(InputType type) const
{
	return IsPressed(type) && !lastInput_[static_cast<int>(type)];
}

XInputTypeStic InputState::IsXInputStic(XInputType stic) const
{
	XINPUT_STATE  padState;
	GetJoypadXInputState(DX_INPUT_PAD1, &padState);
	float x, y;
	if (stic == XInputType::LEFT)
	{
		x = static_cast<float>(padState.ThumbLX / 32767.0f);
		y = static_cast<float>(padState.ThumbLY / 32767.0f);
	}
	else
	{
		x = static_cast<float>(padState.ThumbRX / 32767.0f);
		y = static_cast<float>(padState.ThumbRY / 32767.0f);
	}
	DrawFormatString(300, 300, 0xffffff, "%.2f", x);

	if (x < 0.8f && x > 0.3f)
	{
		// 右にちょっと動く
		return XInputTypeStic::LITTLE_RIGHT;
	}
	else if (x > 0.9f)
	{
		// 右に大きく動く
		return XInputTypeStic::RIGHT;
	}
	else if (x < -0.3f && x > -0.8f)
	{
		// 左に少し動く
		return XInputTypeStic::LITTLE_LEFT;
	}
	else if (x < -0.9f)
	{
		// 左に大きく動く
		return XInputTypeStic::LEFT;
	}

	if (y < 0.8f && y > 0.3f)
	{
		// 上にちょっと動く
		return XInputTypeStic::LITTLE_UP;
	}
	else if (y > 0.9f)
	{
		// 上に大きく動く
		return XInputTypeStic::UP;
	}
	else if (y < -0.3f && y > -0.8f)
	{
		// 下に少し動く
		return XInputTypeStic::LITTLE_DOWN;
	}
	else if (y < -0.9f)
	{
		// 下に大きく動く
		return XInputTypeStic::DOWN;
	}
	// 動いてない
	return XInputTypeStic::NONE;

}

bool InputState::IsXInputTrigger(XInputType type) const
{
	XINPUT_STATE  padState;
	GetJoypadXInputState(DX_INPUT_PAD1, &padState);
	if (type == XInputType::LEFT)
	{
		return 	padState.LeftTrigger > padState.LeftTrigger / 2;
	}
	else
	{
		return 	padState.RightTrigger > padState.RightTrigger / 2;
	}
}