#include "Laser.h"
#include "../Util/Effekseer3DEffectManager.h"
#include "../Util/Debug.h"
#include "../Util/MathUtil.h"

namespace
{
	constexpr VECTOR model_scale = { 0.1f, 0.1f, 0.1f };
	constexpr VECTOR init_model_direction{ 1, 0, 0 };
	constexpr VECTOR init_effect_direction{ 0, 0, -1 };
	constexpr VECTOR effect_scale = { 24.0f, 24.0f, 24.0f };

	// エフェクトのチャージ時間フレーム
	constexpr int effect_charge_frame = 150;

	// エフェクトの総再生時間フレーム
	constexpr int effect_total_play_frame = 290;
}

Laser::Laser(int modelHandle, VECTOR* firePos, VECTOR* vec, float fireFrameTime, bool isInfinity, int chargeFrame) :
	effectPos_({}),
	isReflect_(false)
{
	// TODO : レーザーがプレイヤーを追従するようにする
	// できたけどなんか変、別のゲーム研究して改変予知あり
	
	// TODO : レーザーの反射処理の変更
	// できたけどかくかくだから補完入れて

	// 初期化
	firePos_ = firePos;
	pos_ = *firePos;
	vec_ = vec;
	isInfinity_ = isInfinity;
	fireFrameTime_ = fireFrameTime;
	scale_ = model_scale;
	isEnabled_ = true;

	// ベクトル方向の回転行列を作成
	rotMtx_ = MGetRotVec2(init_model_direction, *vec_);

	// ベクトル方向の回転行列からオイラー角を出力
	MATRIX rotEffectMtx = MGetRotVec2(init_effect_direction, *vec_);
	bool isGimbalLock = false;
	VECTOR effectRot = MathUtil::ToEulerAngles(rotEffectMtx, isGimbalLock);

	// エフェクトのチャージ時間フレームと再生したいフレーム速度から再生速度を求める
	effectSpeed_ = static_cast<float>(effect_charge_frame) / static_cast<float>(chargeFrame);

	// エフェクトのチャージ時間フレームとエフェクトの再生速度からチャージ時間が何フレームなのか求める
	collisionAndEffectDifferenceTimer_ = effect_charge_frame / effectSpeed_;

	auto& effectManager = Effekseer3DEffectManager::GetInstance();
	if (!isInfinity_)
	{
		// レーザーのエフェクトの再生
		effectManager.PlayEffectFollow(laserEffectHandle_, EffectID::normal_laser, firePos_, effect_scale, effectSpeed_, effectRot);
	}
	else
	{	
		// 継続レーザーのエフェクトの再生
		effectManager.PlayEffectFollow(laserEffectHandle_, EffectID::continue_laser, firePos_, effect_scale, effectSpeed_, effectRot);
	}

	// 当たり判定に使用するモデルの設定
	pModel_ = std::make_unique<Model>(modelHandle);	// インスタンス生成
	pModel_->SetUseCollision(true);					// 当たり判定設定
	pModel_->SetScale(scale_);		// 拡大率
	pModel_->SetRotMtx(rotMtx_);	// 回転行列
	pModel_->SetPos(pos_);			// 位置
	pModel_->Update();				// 当たり判定の更新
}

Laser::~Laser()
{
	auto& effectManager = Effekseer3DEffectManager::GetInstance();
	effectManager.DeleteEffect(laserEffectHandle_);
}

void Laser::Update()
{
	// 常に発射位置に追従する
	pos_ = *firePos_;

	// ベクトル方向の回転行列をモデルに設定
	rotMtx_ = MGetRotVec2(init_model_direction, *vec_);

	// ベクトル方向の回転行列からオイラー角を出力
	MATRIX rotEffectMtx = MGetRotVec2(init_effect_direction, *vec_);
	bool isGimbalLock = false;
	VECTOR effectRot = MathUtil::ToEulerAngles(rotEffectMtx, isGimbalLock);

	// エフェクトのチャージが終了したら発射
	collisionAndEffectDifferenceTimer_.Update(1);
	if (collisionAndEffectDifferenceTimer_.IsTimeOut())
	{
		scale_.x = -1.0f;

		if (!isInfinity_)
		{
			// エフェクトの総再生時間と再生したいフレーム時間からエフェクトの再生速度を求める
			effectSpeed_ = static_cast<float>(effect_total_play_frame / fireFrameTime_);
		}
		else
		{
			effectSpeed_ = 1.5f;
		}
	}

	// エフェクトの設定
	auto& effectManager = Effekseer3DEffectManager::GetInstance();
	effectManager.SetEffectRot(laserEffectHandle_, effectRot);			// 回転率の設定
	effectManager.SetEffectScale(laserEffectHandle_, effect_scale);		// 拡大率の設定
	effectManager.SetEffectSpeed(laserEffectHandle_, effectSpeed_);		// スピードの設定

	// モデルの設定
	pModel_->SetRotMtx(rotMtx_);
	pModel_->SetScale(scale_);
	pModel_->SetPos(pos_);
	pModel_->Update();
}

void Laser::Draw()
{
#ifdef _DEBUG
	pModel_->Draw();
#endif
}

void Laser::Stop(const VECTOR pos)
{
	VECTOR vec = VSub(pos, *firePos_);
	float size = VSize(vec);

	auto& effectManager = Effekseer3DEffectManager::GetInstance();
	effectManager.SetEffectScale(laserEffectHandle_, { effect_scale.x, effect_scale.y, size / 310.0f });

	scale_.x = 1.0f;
	pModel_->SetScale(scale_);
	pModel_->Update();
}

// レーザーのエフェクトの再生が終了していたら当たり判定用のモデルを削除
void Laser::ConfirmDelete()
{
	auto& effectManager = Effekseer3DEffectManager::GetInstance();
	if (!effectManager.IsPlayingEffect(laserEffectHandle_))
	{
		isEnabled_ = false;
	}
}

VECTOR Laser::GetVec() const
{
	return *vec_;
}