#include "Background.h"
#include "Util/DrawFunctions.h"
#include <string>
#include <DxLib.h>
#include <cassert>
#include "Util/Model.h"
#include "common.h"

namespace
{
	// �w�i�̓���̃t�@�C���p�X
	const std::string movie_file_path = "Data/Movie/star.mp4";

	// ����̍Đ����x
	constexpr double playback_speed = 20.0;
}

Background::Background()
{
	pModel_ = std::make_unique<Model>("Data/Model/Star.mv1");

	pModel_->SetPos({ 0, 0, 0 });
	pModel_->SetRot({ 0, 0, 0 });
	pModel_->SetScale({ 80, 80, 80 });

	//// �w�i�̓���̃��[�h
	//backgroundMovieH_ = my::MyLoadGraph(movie_file_path.c_str());

	//// �w�i�̓���̍Đ����x�ύX
	//int result = SetPlaySpeedRateMovieToGraph(backgroundMovieH_, playback_speed);
	//assert(result != -1);

	//// �w�i�̓���̍Đ����J�n
	//result = PlayMovieToGraph(backgroundMovieH_, DX_PLAYTYPE_LOOP);
	//assert(result != -1);

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
	// �w�i�̓���̃n���h���̍폜
	DeleteGraph(backgroundMovieH_);
}

void Background::Draw()
{
	// �w�i�̓���̕`��
//	DrawRotaGraph(0, 0, 1.0, 0.0f,backgroundMovieH_, false);
 
	// ���f���̕`��
	pModel_->Draw();
}
