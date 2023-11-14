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
	constexpr double playback_speed = 5.0;
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
