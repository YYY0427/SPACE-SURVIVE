#pragma once

// TODO:定数をまとめずにファイル分けする、それと外部ファイル化
namespace common
{
	// Windowモード設定
#ifdef _DEBUG
	constexpr bool window_mode = true;
#else
	constexpr bool window_mode = false;
#endif

	// Window名
	const char* const title_text = "SPACE_SURVIVE";

	// Windowサイズ
	constexpr int screen_width = 1280;
	constexpr int screen_height = 720;

	// カラーモード
	constexpr int color_depth = 32;		// 32 or 16

	// ゲーム内での1m
	constexpr float one_meter = 100.0f;

	// コンフィグで設定する音量を何個に分けるか
	constexpr int config_volume_num = 5;

	// コンフィグで設定するパッドの感度を何個に分けるか
	constexpr int config_pad_sensitivity_num = 10;
};