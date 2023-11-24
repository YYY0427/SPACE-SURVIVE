#include "NormalLazer.h"
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
}

NormalLazer::NormalLazer(int modelHandle, VECTOR* firePos, VECTOR* vec, float effectSpeed, bool isContinue) :
	collisionAndEffectDifferenceTimer_(effect_charge_frame * effectSpeed),
	effectPos_({}),
	isReflect_(false)
{
	// TODO : レーザーがプレイヤーを追従するようにする
	// できたけどなんか変、別のゲーム研究して改変予知あり
	
	// TODO : レーザーの反射処理の変更
	// できたけどかくかくだから補完入れて


	firePos_ = firePos;
	scale_ = model_scale;
	pos_ = *firePos;
	vec_ = vec;
	isEnabled_ = true;

	// ベクトル方向の回転行列を作成
	rotMtx_ = MGetRotVec2(init_model_direction, *vec_);

	// ベクトル方向の回転行列からオイラー角を出力
	MATRIX rotEffectMtx = MGetRotVec2(init_effect_direction, *vec_);
	bool isGimbalLock = false;
	VECTOR effectRot = MathUtil::ToEulerAngles(rotEffectMtx, isGimbalLock);

	auto& effectManager = Effekseer3DEffectManager::GetInstance();
	if (!isContinue)
	{
		// レーザーのエフェクトの再生
		effectManager.PlayEffectFollow(laserEffectHandle_, EffectID::normal_laser, firePos_, effect_scale, effectSpeed, effectRot);
	}
	else
	{
		// 継続レーザーのエフェクトの再生
		effectManager.PlayEffectFollow(laserEffectHandle_, EffectID::continue_laser, firePos_, effect_scale, effectSpeed, effectRot);
	}

	// 当たり判定に使用するモデルの設定
	pModel_ = std::make_unique<Model>(modelHandle);	// インスタンス生成
	pModel_->SetUseCollision(true);					// 当たり判定設定
	pModel_->SetScale(scale_);		// 拡大率
	pModel_->SetRotMtx(rotMtx_);	// 回転行列
	pModel_->SetPos(pos_);			// 位置
	pModel_->Update();				// 当たり判定の更新
}

NormalLazer::~NormalLazer()
{
	auto& effectManager = Effekseer3DEffectManager::GetInstance();
	effectManager.DeleteEffect(laserEffectHandle_);
}

void NormalLazer::Update()
{
	// 常に発射位置に追従する
	pos_ = *firePos_;

	// ベクトル方向の回転行列をモデルに設定
	rotMtx_ = MGetRotVec2(init_model_direction, *vec_);

	// ベクトル方向の回転行列からオイラー角を出力
	MATRIX rotEffectMtx = MGetRotVec2(init_effect_direction, *vec_);
	bool isGimbalLock = false;
	VECTOR effectRot = MathUtil::ToEulerAngles(rotEffectMtx, isGimbalLock);

	// エフェクトの回転率の設定
	auto& effectManager = Effekseer3DEffectManager::GetInstance();
	effectManager.SetEffectRot(laserEffectHandle_, effectRot);

	// エフェクトの拡大率の設定
	effectManager.SetEffectScale(laserEffectHandle_, effect_scale);

	// エフェクトのチャージが終了したら発射
	collisionAndEffectDifferenceTimer_.Update(1);
	if (collisionAndEffectDifferenceTimer_.IsTimeOut())
	{
		scale_.x = -1.0f;
	}

	// モデルの設定
	pModel_->SetRotMtx(rotMtx_);
	pModel_->SetScale(scale_);
	pModel_->SetPos(pos_);
	pModel_->Update();
}

void NormalLazer::Draw()
{
#ifdef _DEBUG
	pModel_->Draw();
#endif
}

void NormalLazer::Stop(const VECTOR pos)
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
void NormalLazer::ConfirmDelete()
{
	auto& effectManager = Effekseer3DEffectManager::GetInstance();
	if (!effectManager.IsPlayingEffect(laserEffectHandle_))
	{
		isEnabled_ = false;
	}
}

VECTOR NormalLazer::GetVec() const
{
	return *vec_;
}