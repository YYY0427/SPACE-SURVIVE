#include "ReflectLaser.h"
#include "../Util/MathUtil.h"
#include "../Util/Model.h"
#include "../Util/Effekseer3DEffectManager.h"

namespace
{
	constexpr VECTOR init_model_direction{ -1, 0, 0 };
	constexpr VECTOR init_effect_direction{ 0, 0, -1 };
	constexpr VECTOR model_scale = { 1.0f, 0.1f, 0.1f };
	constexpr VECTOR effect_scale = { 40.0f, 40.0f, 24.0f };
}

ReflectLaser::ReflectLaser(int modelHandle, VECTOR pos, VECTOR vec)
{
	pos_ = pos;
	vec_ = vec;
	isEnabled_ = true;

	// ベクトル方向の回転行列からオイラー角を出力
	MATRIX rotEffectMtx = MGetRotVec2(init_effect_direction, VScale(vec_, -1));
	bool isGimbalLock = false;
	VECTOR effectRot = MathUtil::ToEulerAngles(rotEffectMtx, isGimbalLock);

	// エフェクトの再生
	auto& effectManager = Effekseer3DEffectManager::GetInstance();
	effectManager.PlayEffectLoopAndFollow(laserEffectHandle_, EffectID::refrect_laser, &pos_, effect_scale, 1.0f, effectRot);

	// ベクトル方向の回転行列を作成
	rotMtx_ = MGetRotVec2(init_model_direction, vec_);

	// 当たり判定に使用するモデルの設定
	pModel_ = std::make_unique<Model>(modelHandle);	// インスタンス生成
	pModel_->SetUseCollision(true);					// 当たり判定設定
	pModel_->SetScale(model_scale);	// 拡大率
	pModel_->SetRotMtx(rotMtx_);	// 回転行列
	pModel_->SetPos(pos_);			// 位置
	pModel_->Update();				// 当たり判定の更新
}

ReflectLaser::~ReflectLaser()
{
	auto& effectManager = Effekseer3DEffectManager::GetInstance();
	effectManager.DeleteEffect(laserEffectHandle_);
}

void ReflectLaser::Update()
{
	//VECTOR vec = VSub(goalVec_, vec_);
	//vec = VNorm(vec);
	//vec = VScale(vec, 10.0f);

	// ベクトル方向の回転行列からオイラー角を出力
	//MATRIX rotEffectMtx = MGetRotVec2(init_effect_direction, vec);
	//bool isGimbalLock = false;
	//VECTOR effectRot = MathUtil::ToEulerAngles(rotEffectMtx, isGimbalLock);
	//auto& effectManager = Effekseer3DEffectManager::GetInstance();
	//effectManager.SetEffectRot(laserEffectHandle_, effectRot);

	// ベクトル方向の回転行列を作成
	//rotMtx_ = MGetRotVec2(init_model_direction, vec);

	//pModel_->SetRotMtx(rotMtx_);	// 回転行列
	//pModel_->Update();				// 当たり判定の更新
}

void ReflectLaser::Draw()
{
#ifdef _DEBUG
	pModel_->Draw();
#endif
}

void ReflectLaser::ReflectLaserUpdate(VECTOR pos, VECTOR vec)
{
	pos_ = pos;
	goalVec_ = vec;
	vec_ = vec;

	pModel_->SetPos(pos_);			// 位置
	pModel_->Update();				// 当たり判定の更新

	// ベクトル方向の回転行列からオイラー角を出力
	MATRIX rotEffectMtx = MGetRotVec2(init_effect_direction, vec_);
	bool isGimbalLock = false;
	VECTOR effectRot = MathUtil::ToEulerAngles(rotEffectMtx, isGimbalLock);
	auto& effectManager = Effekseer3DEffectManager::GetInstance();
	effectManager.SetEffectRot(laserEffectHandle_, effectRot);

	// ベクトル方向の回転行列を作成
	rotMtx_ = MGetRotVec2(init_model_direction, vec_);

	pModel_->SetRotMtx(rotMtx_);	// 回転行列
	pModel_->SetPos(pos_);			// 位置
	pModel_->Update();				// 当たり判定の更新
}