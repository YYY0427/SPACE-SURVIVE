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
	constexpr int effect_total_play_frame = 100;

	// エフェクトの再生速度
	constexpr float effect_play_speed = 1.5f;
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
	fireFrameTimer_ = fireFrameTime;
	scale_ = model_scale;
	isEnabled_ = true;

	// ベクトル方向の回転行列を作成
	rotMtx_ = MGetRotVec2(init_model_direction, *vec_);

	// ベクトル方向の回転行列からオイラー角を出力
	MATRIX rotEffectMtx = MGetRotVec2(init_effect_direction, *vec_);
	bool isGimbalLock = false;
	VECTOR effectRot = MathUtil::ToEulerAngles(rotEffectMtx, isGimbalLock);

	// エフェクトのチャージ時間フレームとエフェクトの再生速度からチャージ時間が何フレームなのか求める
	chargeEffectTimer_ = effect_charge_frame / effect_play_speed;

	// 無限レーザーのエフェクトの再生
	auto& effectManager = Effekseer3DEffectManager::GetInstance();
	effectManager.PlayEffectFollow(laserEffectHandle_, EffectID::infinity_laser, firePos_, effect_scale, effect_play_speed, effectRot);

	// 当たり判定に使用するモデルの設定
	pModel_ = std::make_unique<Model>(modelHandle);	// インスタンス生成
	pModel_->SetUseCollision(true);					// 当たり判定設定
	pModel_->SetScale(scale_);		// 拡大率
	pModel_->SetRotMtx(rotMtx_);	// 回転行列
	pModel_->SetPos(pos_);			// 位置
	pModel_->SetOpacity(alpha_);	// 不透明度
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
	chargeEffectTimer_.Update(1);
	if (chargeEffectTimer_.IsTimeOut())
	{
		scale_.x = -1.0f;

		// 通常のレーザーの場合
		if (!isInfinity_)
		{
			fireFrameTimer_.Update(1);
			if (fireFrameTimer_.IsTimeOut())
			{
				isEnabled_ = false;
			}
		}
	}

	// エフェクトの設定
	auto& effectManager = Effekseer3DEffectManager::GetInstance();
	effectManager.SetEffectRot(laserEffectHandle_, effectRot);			// 回転率の設定
	effectManager.SetEffectScale(laserEffectHandle_, effect_scale);		// 拡大率の設定

	// モデルの設定
	pModel_->SetRotMtx(rotMtx_);
	pModel_->SetScale(scale_);
	pModel_->SetPos(pos_);
	pModel_->SetOpacity(alpha_);	// 不透明度
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

VECTOR Laser::GetVec() const
{
	return *vec_;
}