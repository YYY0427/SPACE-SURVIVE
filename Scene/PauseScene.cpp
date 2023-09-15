#include "PauseScene.h"
#include "SceneManager.h"
#include "SoundSettingScene.h"
#include "../Util/InputState.h"
#include "../Game.h"
#include <DxLib.h>

namespace
{
	// ポーズウィンドウ
	constexpr int window_width = 700;	//ポーズ枠の幅
	constexpr int window_height = 500;	//ポーズ枠の高さ
	constexpr int window_start_x = (Game::screen_width - window_width) / 2;		//ポーズ枠の左
	constexpr int window_start_y = (Game::screen_height - window_height) / 2;	//ポーズ枠上
}

/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="manager">シーンマネージャーの参照</param>
PauseScene::PauseScene(SceneManager& manager) :
	Scene(manager)
{
}

/// <summary>
/// デストラクタ
/// </summary>
PauseScene::~PauseScene()
{
}

/// <summary>
/// 更新
/// </summary>
void PauseScene::Update()
{
	if (InputState::IsTriggered(InputType::BACK) || InputState::IsTriggered(InputType::PAUSE))
	{
		manager_.PopScene();
		return;
	}
}

/// <summary>
/// 描画
/// </summary>
void PauseScene::Draw()
{
	SetDrawBlendMode(DX_BLENDMODE_MULA, 196);
	DrawBox(window_start_x, window_start_y, window_start_x + window_width, window_start_y + window_height, 0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	//ポーズウィンドウ
	DrawBox(window_start_x, window_start_y, window_start_x + window_width, window_start_y + window_height, 0xd0d0d0, true);
	DrawBox(window_start_x, window_start_y, window_start_x + window_width, window_start_y + window_height, 0xffffff, false);

	//ポーズ中メッセージ
	DrawString(window_start_x + 10, window_start_y + 10, "Pausing...", 0xffff88);
}