#include "Background.h"
#include "Util/DrawFunctions.h"
#include <string>
#include <DxLib.h>
#include <cassert>

namespace
{
	// ”wŒi‚Ì“®‰æ‚Ìƒtƒ@ƒCƒ‹ƒpƒX
	const std::string movie_file_path = "Data/Movie/star.mp4";

	// “®‰æ‚ÌÄ¶‘¬“x
	constexpr double playback_speed = 20.0;
}

Background::Background()
{
	// ”wŒi‚Ì“®‰æ‚Ìƒ[ƒh
	backgroundMovieH_ = my::MyLoadGraph(movie_file_path.c_str());

	// ”wŒi‚Ì“®‰æ‚ÌÄ¶‘¬“x•ÏX
	int result = SetPlaySpeedRateMovieToGraph(backgroundMovieH_, playback_speed);
	assert(result != -1);

	// ”wŒi‚Ì“®‰æ‚ÌÄ¶‚ğŠJn
	result = PlayMovieToGraph(backgroundMovieH_, DX_PLAYTYPE_LOOP);
	assert(result != -1);
}

Background::~Background()
{
	// ”wŒi‚Ì“®‰æ‚Ìƒnƒ“ƒhƒ‹‚Ìíœ
	DeleteGraph(backgroundMovieH_);
}

void Background::Draw()
{
	// ”wŒi‚Ì“®‰æ‚Ì•`‰æ
	DrawGraph(0, 0, backgroundMovieH_, false);
}
