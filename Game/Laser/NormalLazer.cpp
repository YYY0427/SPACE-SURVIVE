#include "NormalLazer.h"
#include "../Util/DataReaderFromUnity.h"
#include "../Util/Effekseer3DEffectManager.h"
#include "../Util/Debug.h"

namespace
{
	constexpr VECTOR model_scale = { 100.0f, 40.0f, 40.0f };
	constexpr VECTOR init_model_direction{ -1, 0, 0 };
}

NormalLazer::NormalLazer(int modelHandle)
{
	// モデルのインスタンスの作成
	pModel_ = std::make_unique<Model>(modelHandle);

	// モデルの拡大率の設定
	pModel_->SetScale(model_scale);
}

NormalLazer::~NormalLazer()
{
}

void NormalLazer::Update()
{
	pos_ = VAdd(pos_, vec_);
	pModel_->SetPos(pos_);
	pModel_->Update();

	/*if (VSize(VSub(firePos_, pos_)) > 2000.0f)
	{
		isEnabled_ = false;
	}*/
}

void NormalLazer::Draw()
{
#ifdef _DEBUG
	pModel_->Draw();
#endif
}

void NormalLazer::Fire(const VECTOR pos, const VECTOR vec, const VECTOR rot)
{
	firePos_ = pos;
	pos_ = pos;
	vec_ = vec;
	isEnabled_ = true;

	// プレイヤーの方向の回転行列の作成
	MATRIX rotMtx = MGetRotVec2(init_model_direction, vec_);
	MV1SetRotationMatrix(pModel_->GetModelHandle(), rotMtx);

	auto& effectManager = Effekseer3DEffectManager::GetInstance();
	effectManager.PlayEffect(lazerEffectHandle_, EffectID::normal_lazer, pos_, 300.0f, 1.0f, rot);
}