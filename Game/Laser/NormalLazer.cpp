#include "NormalLazer.h"
#include "../Util/DataReaderFromUnity.h"
#include "../Util/Effekseer3DEffectManager.h"
#include "../Util/Debug.h"
#include "../MathUtil.h"

namespace
{
	constexpr VECTOR model_scale = { 7.0f, 0.5f, 0.5f };
	constexpr VECTOR init_model_direction{ 1, 0, 0 };
	constexpr VECTOR init_effect_direction{ 0, 0, -1 };
	constexpr float effect_scale = 150.0f;

	constexpr int collision_and_effect_difference_frame = 120;
}

NormalLazer::NormalLazer(int modelHandle, VECTOR* pos, VECTOR vec) :
	collisionAndEffectDifferenceTimer_(collision_and_effect_difference_frame),
	effectPos_({})
{
	// モデルのインスタンスの作成
	pModel_ = std::make_unique<Model>(modelHandle);

	// 当たり判定設定
	pModel_->SetUseCollision(true);

	// モデルの拡大率の設定
	pModel_->SetScale(model_scale);

	firePos_ = pos;
	pos_ = *pos;
	vec_ = vec;
	isEnabled_ = true;

	// ベクトル方向の回転行列をモデルに設定
	MATRIX rotMtx = MGetRotVec2(init_model_direction, vec_);
	MV1SetRotationMatrix(pModel_->GetModelHandle(), rotMtx);

	// ベクトル方向の回転行列からオイラー角を出力
	MATRIX rotEffectMtx = MGetRotVec2(init_effect_direction, vec_);
	bool isGimbalLock = false;
	VECTOR effectRot = MathUtil::ToEulerAngles(rotEffectMtx, isGimbalLock);

	// レーザーのエフェクトの再生
	auto& effectManager = Effekseer3DEffectManager::GetInstance();
	effectManager.PlayEffectFollow(lazerEffectHandle_, EffectID::normal_lazer, firePos_, effect_scale, 1.0f, effectRot);

	pModel_->SetPos(pos_);

	pModel_->Update();
}

NormalLazer::~NormalLazer()
{
}

void NormalLazer::Update(VECTOR scrollVec)
{
	pos_ = VAdd(pos_, scrollVec);
	effectPos_ = VAdd(effectPos_, scrollVec);
	collisionAndEffectDifferenceTimer_.Update(1);
	if (collisionAndEffectDifferenceTimer_.IsTimeOut())
	{
		pos_ = VAdd(pos_, vec_);
		pModel_->SetPos(pos_);
		pModel_->Update();
	}
}

void NormalLazer::Draw()
{
#ifdef _DEBUG
	pModel_->Draw();
#endif
}

void NormalLazer::Refrect(const VECTOR pos, const VECTOR norm)
{
	// レーザーのエフェクトをストップ
	auto& effectManager = Effekseer3DEffectManager::GetInstance();
	effectManager.DeleteEffect(lazerEffectHandle_);

	isRefrect_ = true;
	pos_ = pos;
	effectPos_ = pos;
	pModel_->SetPos(pos);

#if true
	// 反射ベクトルの作成
	VECTOR inversionVec = VScale(vec_, -1);
	float dot = VDot(inversionVec, norm);
	dot *= 2.0f;
	VECTOR normVec = VScale(norm, dot);
	vec_ = VAdd(vec_, normVec);
	vec_ = VNorm(vec_);
	vec_ = VScale(vec_, 300.0f);
#else
	vec_ = VScale(vec_, -1);
#endif

	// ベクトル方向の回転行列をモデルに設定
	MATRIX rotModelMtx = MGetRotVec2(init_model_direction, vec_);
	MV1SetRotationMatrix(pModel_->GetModelHandle(), rotModelMtx);

	// ベクトル方向の回転行列からオイラー角を出力
	MATRIX rotEffectMtx = MGetRotVec2(init_effect_direction, vec_);
	bool isGimbalLock = false;
	VECTOR effectRot = MathUtil::ToEulerAngles(rotEffectMtx, isGimbalLock);

	// レーザーのエフェクトの再生
	effectManager.PlayEffectFollow(lazerEffectHandle_, EffectID::refrect_laser, &effectPos_, effect_scale, 1.0f, effectRot);

	pModel_->Update();
}

void NormalLazer::CheckInCamera()
{
	if (CheckCameraViewClip(pos_))
	{
//		isEnabled_ = false;
	}
}
