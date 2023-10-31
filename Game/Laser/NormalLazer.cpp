#include "NormalLazer.h"
#include "../Util/DataReaderFromUnity.h"
#include "../Util/Effekseer3DEffectManager.h"
#include "../Util/Debug.h"

namespace
{
	constexpr VECTOR model_scale = { 8.0f, 1.0f, 1.0f };
	constexpr VECTOR init_model_direction{ 1, 0, 0 };
	constexpr float effect_scale = 150.0f;

	constexpr int collision_and_effect_difference_frame = 120;
}

NormalLazer::NormalLazer(int modelHandle) :
	collisionAndEffectDifferenceTimer_(collision_and_effect_difference_frame)
{
	// モデルのインスタンスの作成
	pModel_ = std::make_unique<Model>(modelHandle);

	// 当たり判定設定
	pModel_->SetUseCollision(true);

	// モデルの拡大率の設定
	pModel_->SetScale(model_scale);
}

NormalLazer::~NormalLazer()
{
}

void NormalLazer::Update()
{
	collisionAndEffectDifferenceTimer_.Update(1);

	if (collisionAndEffectDifferenceTimer_.IsTimeOut())
	{
		pos_ = VAdd(pos_, vec_);
		pModel_->SetPos(pos_);
		pModel_->Update();
	}
	
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
	rot_ = rot;
	isEnabled_ = true;

	// プレイヤーの方向の回転行列の作成
	MATRIX rotMtx = MGetRotVec2(init_model_direction, vec_);
	MV1SetRotationMatrix(pModel_->GetModelHandle(), rotMtx);

	// レーザーのエフェクトの再生
	auto& effectManager = Effekseer3DEffectManager::GetInstance();
	effectManager.PlayEffect(lazerEffectHandle_, EffectID::normal_lazer, pos_, effect_scale, 1.0f, rot_);
}

void NormalLazer::Refrect()
{
	auto& effectManager = Effekseer3DEffectManager::GetInstance();

	// レーザーのエフェクトをストップ
	effectManager.DeleteEffect(lazerEffectHandle_);

	// ベクトルを反転
	vec_ = VScale(vec_, -1);

	// レーザーのエフェクトの再生
	effectManager.PlayEffect(lazerEffectHandle_, EffectID::refrect_laser, pos_, effect_scale, 1.0f, {rot_.x, rot_.y + DX_PI_F, 0.0f});
}
