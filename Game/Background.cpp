#include "Background.h"
#include "Util/DrawFunctions.h"
#include <string>
#include <DxLib.h>
#include <cassert>

namespace
{
	// �w�i�̓���̃t�@�C���p�X
	const std::string movie_file_path = "Data/Movie/star.mp4";

	// ����̍Đ����x
	constexpr double playback_speed = 20.0;
}

Background::Background()
{
	// �w�i�̓���̃��[�h
	backgroundMovieH_ = my::MyLoadGraph(movie_file_path.c_str());

	// �w�i�̓���̍Đ����x�ύX
	int result = SetPlaySpeedRateMovieToGraph(backgroundMovieH_, playback_speed);
	assert(result != -1);

	// �w�i�̓���̍Đ����J�n
	result = PlayMovieToGraph(backgroundMovieH_, DX_PLAYTYPE_LOOP);
	assert(result != -1);
}

Background::~Background()
{
	// �w�i�̓���̃n���h���̍폜
	DeleteGraph(backgroundMovieH_);
}

void Background::Draw()
{
	// �w�i�̓���̕`��
	DrawGraph(0, 0, backgroundMovieH_, false);
}
