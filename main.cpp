#include <DxLib.h>
#include "EffekseerForDXLib.h"
#include "Game.h"
#include "Util/SoundManager.h"
#include "Scene/SceneManager.h"
#include "Scene/TitleScene.h"
#include "Scene/DebugScene.h"
#include "Util/Effekseer3DEffectManager.h"
#include "Util/SaveData.h"
#include "Util/InputState.h"

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
	SetChangeScreenModeGraphicsSystemResetFlag(TRUE);

	// ほかのウィンドウを選択していても動くようにする
	SetAlwaysRunFlag(TRUE);

	// ウィンドウのサイズを変更可能にする
	SetWindowSizeChangeEnableFlag(TRUE);

	// DirectX11を使用するようにする。(DirectX9も可)
	// Effekseerを使用するには必ず設定する。
	SetUseDirect3DVersion(DX_DIRECT3D_11);
	
	// １メートルに相当する値を設定する
	Set3DSoundOneMetre(Game::one_meter);

	// XAudioを有効化
	SetEnableXAudioFlag(TRUE);

	// 垂直同期を有効化
	SetWaitVSyncFlag(TRUE);

	// ＤＸライブラリ初期化処理
	if (DxLib_Init() == -1)
	{
		// エラーが起きたら止める
		assert(0);
	}
	// Effekseerの初期化
	auto& effectManager = Effekseer3DEffectManager::GetInstance();
	if (effectManager.Init() == -1)
	{
		// エラーが起きたら止める
		assert(0);
	}

	// フルスクリーンウインドウの切り替えでリソースが消えるのを防ぐ
	// Effekseerを使用する場合は必ず設定する
	SetChangeScreenModeGraphicsSystemResetFlag(FALSE);

	// DXライブラリのデバイスロストした時のコールバックを設定する
	// ウインドウとフルスクリーンの切り替えが発生する場合は必ず実行する
	// ただし、DirectX11を使用する場合は実行する必要はない
	Effekseer_SetGraphicsDeviceLostCallbackFunctions();
	
	// Zバッファを有効化
	SetUseZBufferFlag(TRUE);

	// Zバッファへの書き込みを行う
	SetWriteZBuffer3D(TRUE);

	// ポリゴンの裏面を描画しない
	SetUseBackCulling(TRUE);

	// 重力の設定
	MV1SetLoadModelPhysicsWorldGravity(-9.8f);
	
	// セーブデータの読み込み
	auto& saveData = SaveData::GetInstance();
	saveData.Load();

	// csvファイルに沿ってサウンドをロード
	auto& soundManager = SoundManager::GetInstance();
	soundManager.LoadAndSaveSoundFileData();

	// 入力タイプの初期化
	InputState::Init();

	// ダブルバッファモード
	// 裏画面に描画
	SetDrawScreen(DX_SCREEN_BACK);
	
	// 初期シーンの設定
	SceneManager sceneManager;
#ifdef _DEBUG
	sceneManager.ChangeScene(new DebugScene(sceneManager));
#else 
	sceneManager.ChangeScene(new TitleScene(sceneManager));
#endif

	// 異常が起きた時に終了
	while (ProcessMessage() == 0)
	{
		LONGLONG time = GetNowHiPerformanceCount();

		// 画面のクリア
		ClearDrawScreen();

		InputState::Update();
		sceneManager.Update();
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

	// Effekseerの終了処理
	effectManager.End();

	// ＤＸライブラリ使用の終了処理
	DxLib_End();

	// ソフトの終了 
	return 0;
}
