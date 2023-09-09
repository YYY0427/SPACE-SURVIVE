#include <DxLib.h>
#include "Game.h"
#include "InputState.h"
#include "Util/SoundManager.h"
#include "Scene/SceneManager.h"
#include "Scene/TitleScene.h"
#include "Util/Effekseer3DEffectManager.h"
#include "EffekseerForDXLib.h"

// プログラムは WinMain から始まります
int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_  HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd)
{
	// Windowモード設定
	ChangeWindowMode(Game::window_mode);

	// Window名設定
	SetMainWindowText(Game::title_text);

	// 画面サイズの設定
	SetGraphMode(Game::screen_width, Game::screen_height, Game::color_depth);

	// ゲーム中にウィンドウモードを切り替えてもグラフィックハンドルをリセットしない
	SetChangeScreenModeGraphicsSystemResetFlag(false);

	// ほかのウィンドウを選択していても動くようにする
	SetAlwaysRunFlag(false);

	// ウィンドウのサイズを変更可能にする
	SetWindowSizeChangeEnableFlag(true);

	// DirectX11を使用するようにする。(DirectX9も可)
	// Effekseerを使用するには必ず設定する。
	SetUseDirect3DVersion(DX_DIRECT3D_11);

	// フルスクリーンウインドウの切り替えでリソースが消えるのを防ぐ
	// Effekseerを使用する場合は必ず設定する
	SetChangeScreenModeGraphicsSystemResetFlag(FALSE);

	// DXライブラリのデバイスロストした時のコールバックを設定する
	// ウインドウとフルスクリーンの切り替えが発生する場合は必ず実行する
	// ただし、DirectX11を使用する場合は実行する必要はない
	Effekseer_SetGraphicsDeviceLostCallbackFunctions();

	// XAudio を有効化
	SetEnableXAudioFlag(TRUE);

	// １メートルに相当する値を設定する
	Set3DSoundOneMetre(Game::one_meter);

	auto& soundManager = SoundManager::GetInstance();
	auto& effectManager = Effekseer3DEffectManager::GetInstance();

	// ＤＸライブラリ初期化処理
	if (DxLib_Init() == -1)
	{
		// エラーが起きたら直ちに終了
		return -1;
	}
	// Effekseerの初期化
	if (effectManager.Init() == -1)
	{
		// エラーが起きたら直ちに終了
		return -1;
	}
	
	// csvファイルに沿ってサウンドをロード
	soundManager.LoadAndSaveSoundFileData();

	// ダブルバッファモード
	// 裏画面に描画
	SetDrawScreen(DX_SCREEN_BACK);
	
	InputState input;
	SceneManager sceneManager;
	sceneManager.ChangeScene(new TitleScene(sceneManager));

	// 異常が起きた時に終了
	while (ProcessMessage() == 0)
	{
		LONGLONG time = GetNowHiPerformanceCount();

		// 画面のクリア
		ClearDrawScreen();

		input.Update();
		sceneManager.Update(input);
		effectManager.Update();

		sceneManager.Draw();
		effectManager.Draw();

		// FPS(Frame Per Second)の取得
		auto fps = GetFPS();

		// 描画命令数の取得
		auto drawcall = GetDrawCallCount();

		DrawFormatString(10, 30, 0xffffff, "FPS = %2.2f", fps);
		DrawFormatString(10, 60, 0xffffff, "DC = %d", drawcall);

		// 裏画面を表画面を入れ替える
		ScreenFlip();

		// escキーを押したら終了する
		if (CheckHitKey(KEY_INPUT_ESCAPE))	break;

		// fpsを60に固定
		while (GetNowHiPerformanceCount() - time < 16667)
		{

		}
	}

	effectManager.End();

	// ＤＸライブラリ使用の終了処理
	DxLib_End();

	// ソフトの終了 
	return 0;
}
