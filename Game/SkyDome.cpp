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
SkyDome::SkyDome()
{
	// インスタンスの作成
	pModel_ = std::make_shared<Model>(model_file_path);

	// モデルの拡大率の設定
	pModel_->SetScale(VGet(model_scale, model_scale, model_scale));
}

// デストラクタ
SkyDome::~SkyDome()
{
}

// 更新
void SkyDome::Update(VECTOR playerPos)
{
	// 位置情報の設定
	pModel_->SetPos(playerPos);
}

// 描画
void SkyDome::Draw()
{
	pModel_->Draw();
}