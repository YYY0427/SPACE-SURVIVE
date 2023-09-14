#include "Pad.h"
#include <DxLib.h>
#include <vector>

namespace
{
	// ログ記録フレーム数
	constexpr int kLogNum = 16;
	// パッド最大数
	constexpr int kMaxPad = 2;
	// 
	constexpr int kUsePadData[kMaxPad] =
	{
		DX_INPUT_KEY_PAD1,
		DX_INPUT_PAD2
	};

	// 入力ログ	0が最新の状態
	int padLog[kMaxPad][kLogNum];

	// キーログ
	bool				isRecordLog = false;
	int					playLogNo = -1;	// キーログ再生フレーム数
	std::vector<int>	keyLog;
}


namespace Pad
{
	void Update()
	{
		DINPUT_JOYSTATE input;
		GetJoypadDirectInputState(DX_INPUT_PAD1, &input);
	}// パッドの入力状態取得
	void update()
	{
		for (int padNo = 0; padNo < kMaxPad; padNo++)
		{
			// 現在のパッドの状態を取得
			int padState = GetJoypadInputState(kUsePadData[padNo]);
			if ((playLogNo >= 0) && (padNo == 0))
			{
				if (keyLog.size() > playLogNo)
				{
					padState = keyLog[playLogNo];
					playLogNo++;
				}
				else
				{
					playLogNo = -1;
				}
			}

			// ログの更新
			for (int i = kLogNum - 1; i >= 1; i--)
			{
				padLog[padNo][i] = padLog[padNo][i - 1];
			}
			// 最新の状態
			padLog[padNo][0] = padState;

			// キーログ
			if (isRecordLog)
			{
				if (padNo == 0)
				{
					keyLog.push_back(padState);
				}
			}
		}
	}
}