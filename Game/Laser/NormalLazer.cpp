#include "NormalLazer.h"
#include "../Util/Effekseer3DEffectManager.h"
#include "../Util/Debug.h"
#include "../MathUtil.h"

namespace
{
	constexpr VECTOR model_scale = { 1.12f, 0.1f, 0.1f };
	constexpr VECTOR init_model_direction{ 1, 0, 0 };
	constexpr VECTOR init_effect_direction{ 0, 0, -1 };
	constexpr float effect_scale = 24.0f;

	constexpr int collision_and_effect_difference_frame = 120;

	constexpr float move_speed = 33.0f;
}

NormalLazer::NormalLazer(int modelHandle, VECTOR* pos, VECTOR* vec, VECTOR* enemyMoveVec) :
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
	enemyMoveVec_ = enemyMoveVec;

	// ベクトル方向の回転行列をモデルに設定
	MATRIX rotMtx = MGetRotVec2(init_model_direction, *vec_);
	MV1SetRotationMatrix(pModel_->GetModelHandle(), rotMtx);

	// ベクトル方向の回転行列からオイラー角を出力
	MATRIX rotEffectMtx = MGetRotVec2(init_effect_direction, *vec_);
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

void NormalLazer::Update()
{
	collisionAndEffectDifferenceTimer_.Update(1);
	if (collisionAndEffectDifferenceTimer_.IsTimeOut())
	{
		pos_ = VAdd(pos_, *enemyMoveVec_);

		pos_ = VAdd(pos_, a_);
	}
	else
	{
		a_ = VScale(*vec_, move_speed);
		pos_ = *firePos_;

		// ベクトル方向の回転行列をモデルに設定
		MATRIX rotMtx = MGetRotVec2(init_model_direction, *vec_);
		MV1SetRotationMatrix(pModel_->GetModelHandle(), rotMtx);

		// ベクトル方向の回転行列からオイラー角を出力
		MATRIX rotEffectMtx = MGetRotVec2(init_effect_direction, *vec_);
		bool isGimbalLock = false;
		VECTOR effectRot = MathUtil::ToEulerAngles(rotEffectMtx, isGimbalLock);

		auto& effectManager = Effekseer3DEffectManager::GetInstance();
		effectManager.SetEffectRot(lazerEffectHandle_, effectRot);
	}

	pModel_->SetPos(pos_);
	pModel_->Update();
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
	VECTOR inversionVec = VScale(*vec_, -1);
	float dot = VDot(inversionVec, norm);
	dot *= 2.0f;
	VECTOR normVec = VScale(norm, dot);
	a_ = VAdd(*vec_, normVec);
	a_ = VScale(a_, 48.0f);
#else
	vec_ = VScale(vec_, -1);
#endif

	// ベクトル方向の回転行列をモデルに設定
	MATRIX rotModelMtx = MGetRotVec2(init_model_direction, a_);
	MV1SetRotationMatrix(pModel_->GetModelHandle(), rotModelMtx);

	// ベクトル方向の回転行列からオイラー角を出力
	MATRIX rotEffectMtx = MGetRotVec2(init_effect_direction, a_);
	bool isGimbalLock = false;
	VECTOR effectRot = MathUtil::ToEulerAngles(rotEffectMtx, isGimbalLock);

	// レーザーのエフェクトの再生
	effectManager.PlayEffectFollow(lazerEffectHandle_, EffectID::refrect_laser, &effectPos_, effect_scale, 1.0f, effectRot);

	pModel_->Update();
}

// レーザーのエフェクトの再生が終了していたら当たり判定用のモデルを削除
void NormalLazer::ConfirmDelete()
{
	auto& effectManager = Effekseer3DEffectManager::GetInstance();
	if (!effectManager.IsPlayingEffect(lazerEffectHandle_))
	{
		isEnabled_ = false;
	}
}
