#include "Scene.h"
#include "../Game.h"
#include <cmath>

namespace
{
	constexpr int fade_normal_speed = 8;
}

/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="manager"></param>
Scene::Scene(SceneManager& manager) :
	manager_(manager)
{
	isFadeOut_ = false;
	fadeColor_ = GetColor(255, 0, 0);
	fadeBright_ = 255;
	fadeSpeed_ = -fade_normal_speed;
}

/// <summary>
/// フェードの更新
/// </summary>
void Scene::UpdateFade()
{
	fadeBright_ += fadeSpeed_;

	// フェードアウト終了処理
	if (fadeBright_ >= 255)
	{
		fadeBright_ = 255;
		if (fadeSpeed_ > 0)
		{
			fadeSpeed_ = 0;
		}
	}
	// フェードイン終了処理
	if (fadeBright_ <= 0)
	{
		fadeBright_ = 0;
		if (fadeSpeed_ < 0)
		{
			fadeSpeed_ = 0;
		}
	}
}

/// <summary>
/// フェードの描画
/// </summary>
void Scene::DrawFade()
{
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, fadeBright_);
	DrawBox(0, 0, Game::screen_width, Game::screen_height, fadeColor_, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

/// <summary>
/// フェードアウトの開始
/// </summary>
void Scene::StartFadeOut()
{
	fadeSpeed_ = fade_normal_speed;
}

/// <summary>
/// フェードイン中かどうか
/// </summary>
/// <returns>true : フェードイン中、false : フェードインしていない</returns>
bool Scene::IsFadingIn() const
{
	if (fadeSpeed_ < 0)
	{
		return true;
	}
	return false;
}

/// <summary>
/// フェードアウト中かどうか
/// </summary>
/// <returns>true : フェードアウト中、false : フェードアウトしていない</returns>
bool Scene::IsFadingOut() const
{
	if (fadeSpeed_ > 0)
	{
		return true;
	}
	return false;
}

/// <summary>
/// フェード中かどうか
/// </summary>
/// <returns>true : フェード中、false : フェードしてない</returns>
bool Scene::IsFadeing() const
{
	return IsFadingIn() || IsFadingOut();
}

/// <summary>
/// フェードについての設定(フェード中でしか設定をおこなわない)
/// </summary>
/// <param name="fadeSpeed">フェードの速度</param>
/// <param name="fadeColor">フェードの色(0~255)</param>
/// <param name="fadeBright">フェードの明るさ(0~255)</param>
void Scene::SetFadeConfig(int fadeSpeed, VECTOR fadeColor, int fadeBright)
{
	// フェード中しか設定をおこなわない
	if (!IsFadeing()) return;

	// フェードインかフェードアウトかで値を変更する
	int absoluteFadeSpeed = abs(fadeSpeed);
	if (IsFadingIn())	absoluteFadeSpeed *= -1;

	// ありえない値がはいらないように制限
	if (fadeBright > 255)	fadeBright = 255;
	if (fadeBright < 0)		fadeBright = 0;
	if (fadeColor.x > 255)	fadeColor.x = 255;
	if (fadeColor.x < 0)	fadeColor.x = 0;
	if (fadeColor.y > 255)	fadeColor.y = 255;
	if (fadeColor.y < 0)	fadeColor.y = 0;
	if (fadeColor.z > 255)	fadeColor.z = 255;
	if (fadeColor.z < 0)	fadeColor.z = 0;

	// 値の設定
	fadeSpeed_ = absoluteFadeSpeed;
	fadeColor_ = GetColor(static_cast<int>(fadeColor.x), static_cast<int>(fadeColor.y), static_cast<int>(fadeColor.z));
	fadeBright_ = fadeBright;
}

/// <summary>
/// フェードの明るさを取得
/// </summary>
/// <returns>0(フェードしていない)～255(真っ黒)</returns>
int Scene::GetFadeBright() const
{
	return fadeBright_;
}
