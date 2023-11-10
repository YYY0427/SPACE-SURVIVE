#include "SkyDome.h"
#include "Player.h"
#include "Util/Model.h"
#include <string>

namespace
{
	// スカイドームのモデルのファイルパス
	const std::string model_file_path = "Data/Model/MV1/SkyDome.mv1";

	// モデルの拡大率
	constexpr float model_scale = 2000.0f;
}

// コンストラクタ
SkyDome::SkyDome(VECTOR initPos) :
	pos_(initPos)
{
	// インスタンスの作成
	pModel_ = std::make_shared<Model>(model_file_path);

	// モデルの拡大率の設定
	pModel_->SetScale(VGet(model_scale, model_scale, model_scale));

	// 位置情報の設定
	pModel_->SetPos(pos_);
}

// デストラクタ
SkyDome::~SkyDome()
{
}

// 更新
void SkyDome::Update(VECTOR pos)
{
	// 位置情報の設定
	pos_ = VAdd(pos_, pos);
	pModel_->SetPos(pos_);
	pModel_->Update();
}

// 描画
void SkyDome::Draw()
{
	SetUseLighting(FALSE);
	pModel_->Draw();
	SetUseLighting(TRUE);
}