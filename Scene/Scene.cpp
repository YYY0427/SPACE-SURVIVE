#include "Scene.h"
#include "../common.h"
#include <cmath>
#include "../Util/Range.h"

namespace
{
	// 通常のフェードの速度
	constexpr int fade_normal_speed = 8;

	// モザイクパラメーターの最大値
	constexpr int gauss_max_value = 1400;
}

// コンストラクタ
Scene::Scene(SceneManager& manager) :
	manager_(manager),
	isFadeOut_(false),
	fadeColor_(GetColor(0, 0, 0)),
	fadeBright_(255),
	fadeSpeed_(-fade_normal_speed)
{
	gaussScreen_ = MakeScreen(common::screen_width, common::screen_height);
}

// デストラクタ
Scene::~Scene()
{
	// 処理なし
}

// フェードの更新
void Scene::UpdateFade()
{
	// フェードの明るさの更新
	fadeBright_ += fadeSpeed_;

	// フェードの明るさの下限値と上限値の設定
	const Range<int> fadeBrightRange(0, 255);
	
	// フェードの明るさが設定した範囲を超えたらフェードを止める
	if (!fadeBrightRange.IsInside(fadeBright_))	fadeSpeed_ = 0;

	// フェードの明るさを設定した範囲内にクランプする
	fadeBright_ = fadeBrightRange.Clamp(fadeBright_);


}

// フェードの描画
void Scene::DrawFade()
{
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, fadeBright_);
	DrawBox(0, 0, common::screen_width, common::screen_height, fadeColor_, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

// モザイクフェードの描画
void Scene::DrawGaussFade()
{
	// 0~255の範囲を0~モザイクパラメーターの最大値に変換
	int gaussParameter = fadeBright_ * gauss_max_value / 255;

	// モザイク画像の作成
	GetDrawScreenGraph(0, 0, common::screen_width, common::screen_height, gaussScreen_);
	GraphFilter(gaussScreen_, DX_GRAPH_FILTER_GAUSS, 8, gaussParameter);
	DrawGraph(0, 0, gaussScreen_, true);
}

// フェードアウトの開始
void Scene::StartFadeOut(int fadeSpeed)
{
	// フェードアウトが行われたかどうかのフラグを立てる
	isFadeOut_ = true;

	// フェード速度の設定
	fadeSpeed_ = abs(fadeSpeed);;
}

// フェードインの開始
void Scene::StartFadeIn(int fadeSpeed)
{
	// 初期化
	isFadeOut_ = false;

	// フェード速度の設定
	fadeSpeed_ = -abs(fadeSpeed);
}

// フェードイン中かどうか
bool Scene::IsFadingIn() const
{
	return (fadeSpeed_ < 0);
}

// フェードアウト中かどうか
bool Scene::IsFadingOut() const
{
	return (fadeSpeed_ > 0);
}

// フェード中かどうか
bool Scene::IsFadeing() const
{
	return IsFadingIn() || IsFadingOut();
}

// フェードアウトスタート後にフェードアウト中ではないか 
bool Scene::IsStartFadeOutAfterFadingOut()
{
	return !IsFadingOut() && isFadeOut_;
}

/// <summary>
/// フェードについての設定(フェード中でしか設定をおこなわない)
/// </summary>
/// <param name="fadeSpeed">フェードの速度</param>
/// <param name="fadeColor">フェードの色(0~255)</param>
/// <param name="fadeBright">フェードの明るさ(0~255)</param>
//void Scene::SetFadeConfig(int fadeSpeed, VECTOR fadeColor, int fadeBright)
//{
//	// フェード中しか設定をおこなわない
//	if (!IsFadeing()) return;
//
//	// フェードインかフェードアウトかでフェード速度の値を変更する
//	int absoluteFadeSpeed = abs(fadeSpeed);
//	if (IsFadingIn())	absoluteFadeSpeed *= -1;
//
//	// ありえない値が入らないように制限
//	if (fadeBright > 255)	fadeBright = 255;
//	if (fadeBright < 0)		fadeBright = 0;
//	if (fadeColor.x > 255)	fadeColor.x = 255;
//	if (fadeColor.x < 0)	fadeColor.x = 0;
//	if (fadeColor.y > 255)	fadeColor.y = 255;
//	if (fadeColor.y < 0)	fadeColor.y = 0;
//	if (fadeColor.z > 255)	fadeColor.z = 255;
//	if (fadeColor.z < 0)	fadeColor.z = 0;
//
//	// 値の設定
//	fadeSpeed_ = absoluteFadeSpeed;
//	fadeColor_ = GetColor(static_cast<int>(fadeColor.x), static_cast<int>(fadeColor.y), static_cast<int>(fadeColor.z));
//	fadeBright_ = fadeBright;
//}
