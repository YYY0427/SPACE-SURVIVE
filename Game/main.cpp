#include <DxLib.h>
#include "EffekseerForDXLib.h"
#include "common.h"
#include "Util/SoundManager.h"
#include "Scene/SceneManager.h"
#include "Scene/TitleScene.h"
#include "Scene/DebugScene.h"
#include "Util/Effekseer3DEffectManager.h"
#include "Util/SaveData.h"
#include "Util/InputState.h"
#include "Util/StringManager.h"
#include "Util/Debug.h"

// プログラムは WinMain から始まります
int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_  HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd)
{
	// セーブデータの読み込み
	auto& saveData = SaveData::GetInstance();
	saveData.Load();

	// Windowモード設定
	ChangeWindowMode(saveData.GetSaveData().windowMode);

	// Window名設定
	SetMainWindowText(common::title_text);

	// 画面サイズの設定
	SetGraphMode(common::screen_width, common::screen_height, common::color_depth);

	// ゲーム中にウィンドウモードを切り替えてもグラフィックハンドルをリセットしない
	SetChangeScreenModeGraphicsSystemResetFlag(TRUE);

	// ほかのウィンドウを選択していても動くようにする
	SetAlwaysRunFlag(TRUE);

	// ウィンドウのサイズを変更可能にする
	SetWindowSizeChangeEnableFlag(TRUE);

	// DirectX11を使用
	SetUseDirect3DVersion(DX_DIRECT3D_11);
	
	// １メートルに相当する値を設定する
	Set3DSoundOneMetre(common::one_meter);

	// XAudioを有効化
	SetEnableXAudioFlag(TRUE);

	// 垂直同期を有効化
	SetWaitVSyncFlag(TRUE);

	// 3Dでの描画処理のサポート
	SetUseLarge3DPositionSupport(TRUE);

	// ＤＸライブラリ初期化処理
	if (DxLib_Init() == -1)
	{
		// エラーが起きたら止める
		assert(0);
	}
	// 非同期読み込み設定に変更
//	SetUseASyncLoadFlag(TRUE);

	// Effekseerの初期化
	auto& effectManager = Effekseer3DEffectManager::GetInstance();
	effectManager.Init();

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
//	MV1SetLoadModelPhysicsWorldGravity(-9.8f);
	
	// csvファイルに沿ってサウンドをロード
	auto& soundManager = SoundManager::GetInstance();
	soundManager.LoadAndStoreSoundFileData();

	// csvファイルに沿って文字列をロード
	auto& stringManager = StringManager::GetInstance();
	stringManager.LoadAndStoreStringFileData();

	// 入力タイプの初期化
	InputState::Init();

	// ダブルバッファモード
	// 裏画面に描画
	SetDrawScreen(DX_SCREEN_BACK);
	
	// 初期シーンの設定
	SceneManager sceneManager;
#ifdef _DEBUG
	sceneManager.ChangeScene(new TitleScene(sceneManager));
#else 
#endif
	sceneManager.ChangeScene(new DebugScene(sceneManager));

	// 異常が起きた時に終了
	while (ProcessMessage() == 0)
	{
		LONGLONG time = GetNowHiPerformanceCount();
		Debug::ClearLog();

		// 画面のクリア
		ClearDrawScreen();

		// 更新
		InputState::Update();
		sceneManager.Update();

		// 描画
		sceneManager.Draw();

		// Windowモード設定
		ChangeWindowMode(saveData.GetSaveData().windowMode);

		// FPS(Frame Per Second)の取得
		auto fps = GetFPS();
		Debug::Log("FPS", fps);

		// 描画命令数の取得
		auto drawcall = GetDrawCallCount();
		Debug::Log("DC", drawcall);

		// デバッグログの描画
		// すべての描画後に描画
		Debug::DrawLog();

		// 裏画面を表画面を入れ替える
		ScreenFlip();

		// ゲームを終了するかのフラグのチェック
		// フラグが立っていた場合ループを抜けてゲームを終了
		if (sceneManager.GetIsGameEnd())	break;

#ifdef _DEBUG
		// escキーを押したら終了する
		if (CheckHitKey(KEY_INPUT_ESCAPE))	break;
#endif
		// fpsを固定
		while (GetNowHiPerformanceCount() - time < (static_cast<long long>((1000 / common::fps)) * 1000))
		{

		}
	}

	// 終了処理
	soundManager.StopAllSound();
	stringManager.End();
	effectManager.End();
	DxLib_End();

	// ソフトの終了 
	return 0;
}
