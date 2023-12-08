#include "SceneBase.h"
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
SceneBase::SceneBase(SceneManager& manager) :
	manager_(manager),
	isFadeOut_(false),
	fadeColor_(GetColor(0, 0, 0)),
	fadeBright_(255),
	fadeSpeed_(-fade_normal_speed),
	fadeBrightUpperLimitValue_(255)
{
	// モザイク処理用のグラフィックの作成
	gaussScreen_ = MakeScreen(common::screen_width, common::screen_height);
}

// デストラクタ
SceneBase::~SceneBase()
{
	DeleteGraph(gaussScreen_);
}

// フェードの更新
void SceneBase::UpdateFade()
{
	// フェードの明るさの更新
	fadeBright_ += fadeSpeed_;

	// フェードの明るさの下限値と上限値の設定
	const Range<int> fadeBrightRange(0, fadeBrightUpperLimitValue_);
	
	// フェードの明るさが設定した範囲を超えたらフェードを止める
	if (!fadeBrightRange.IsInside(fadeBright_))	fadeSpeed_ = 0;

	// フェードの明るさを設定した範囲内にクランプする
	fadeBright_ = fadeBrightRange.Clamp(fadeBright_);
}

// フェードの描画
void SceneBase::DrawFade(bool isPlay)
{
	if (!isPlay) return;

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, fadeBright_);
	DrawBox(0, 0, common::screen_width, common::screen_height, fadeColor_, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

// モザイクフェードの描画
void SceneBase::DrawGaussFade(bool isPlay)
{
	if (!isPlay) return;

	// 0~255の範囲を0~モザイクパラメーターの最大値に変換
	int gaussParameter = fadeBright_ * gauss_max_value / 255;

	// モザイク画像の作成
	GetDrawScreenGraph(0, 0, common::screen_width, common::screen_height, gaussScreen_);
	GraphFilter(gaussScreen_, DX_GRAPH_FILTER_GAUSS, 8, gaussParameter);
	DrawGraph(0, 0, gaussScreen_, true);
}

// フェードアウトの開始
void SceneBase::StartFadeOut(int fadeBrightUpperLimitValue, int fadeSpeed)
{
	// フェードアウトをどのくらいまで行うのか値を設定
	// 0(フェードしない)～255(最後までフェードを行う)
	fadeBrightUpperLimitValue_ = fadeBrightUpperLimitValue;

	// フェードアウトが行われたかどうかのフラグを立てる
	isFadeOut_ = true;

	// フェード速度の設定
	fadeSpeed_ = abs(fadeSpeed);;
}

// フェードインの開始
void SceneBase::StartFadeIn(int fadeSpeed)
{
	// 初期化
	isFadeOut_ = false;

	// フェード速度の設定
	fadeSpeed_ = -abs(fadeSpeed);
}

// フェードイン中かどうか
bool SceneBase::IsFadingIn() const
{
	return (fadeSpeed_ < 0);
}

// フェードアウト中かどうか
bool SceneBase::IsFadingOut() const
{
	return (fadeSpeed_ > 0);
}

// フェード中かどうか
bool SceneBase::IsFadeing() const
{
	return IsFadingIn() || IsFadingOut();
}

// フェードアウトスタート後にフェードアウト中ではないか 
bool SceneBase::IsStartFadeOutAfterFadingOut()
{
	return !IsFadingOut() && isFadeOut_;
}

// フェードの明るさの取得
int SceneBase::GetFadeBright() const
{
	return fadeBright_;
}

// フェードの明るさの設定
void SceneBase::SetFadeBright(int fadeBright)
{
	fadeBright_ = fadeBright;
}