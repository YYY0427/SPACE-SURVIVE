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
	// ”wŒi‚Ì“®‰æ‚Ìƒnƒ“ƒhƒ‹‚Ìíœ
	DeleteGraph(backgroundMovieH_);
}

void Background::Draw()
{
	// ”wŒi‚Ì“®‰æ‚Ì•`‰æ
	DrawGraph(0, 0, backgroundMovieH_, false);
}
