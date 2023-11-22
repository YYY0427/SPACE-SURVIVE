#include "Background.h"
#include "Util/DrawFunctions.h"
#include <string>
#include <DxLib.h>
#include <cassert>

namespace
{
	// 背景の動画のファイルパス
	const std::string movie_file_path = "Data/Movie/star.mp4";

	// 動画の再生速度
	constexpr double playback_speed = 20.0;
}

Background::Background()
{
	// 背景の動画のロード
	backgroundMovieH_ = my::MyLoadGraph(movie_file_path.c_str());

	// 背景の動画の再生速度変更
	int result = SetPlaySpeedRateMovieToGraph(backgroundMovieH_, playback_speed);
	assert(result != -1);

	// 背景の動画の再生を開始
	result = PlayMovieToGraph(backgroundMovieH_, DX_PLAYTYPE_LOOP);
	assert(result != -1);

	/*Effekseer3DEffectManager::GetInstance().PlayEffectLoop(
		windEffectH_,
		EffectID::wind,
		{ pPlayer_->GetPos().x, pPlayer_->GetPos().y, pPlayer_->GetPos().z + 500.0f},
		200.0f,
		0.7f,
		{ 0.0f, -DX_PI_F / 2, 0.0f});
	*/
}

Background::~Background()
{
	// 背景の動画のハンドルの削除
	DeleteGraph(backgroundMovieH_);
}

void Background::Draw()
{
	// 背景の動画の描画
	DrawGraph(0, 0, backgroundMovieH_, false);
}
