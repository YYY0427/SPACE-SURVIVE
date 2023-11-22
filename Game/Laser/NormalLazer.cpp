#include "NormalLazer.h"
#include "../Util/Effekseer3DEffectManager.h"
#include "../Util/Debug.h"
#include "../Util/MathUtil.h"

namespace
{
	constexpr VECTOR model_scale = { 1.12f, 0.1f, 0.1f };
	constexpr VECTOR init_model_direction{ 1, 0, 0 };
	constexpr VECTOR init_effect_direction{ 0, 0, -1 };
	constexpr VECTOR effect_scale = { 24.0f, 24.0f, 24.0f };

	constexpr int collision_and_effect_difference_frame = 120;

	constexpr float move_speed = 33.0f;
}

NormalLazer::NormalLazer(int modelHandle, VECTOR* firePos, VECTOR* vec, VECTOR* fireObjectMoveVec, bool isContinue) :
	collisionAndEffectDifferenceTimer_(collision_and_effect_difference_frame),
	effectPos_({})
{
	// TODO : 継続レーザーの処理を書く
	// エフェクトの再生時間によってモデルのxの拡大率を伸ばす
	// 今のエフェクトの再生時間と今のモデルの拡大率から割合だしてやってね
	// がんばれ



	firePos_ = firePos;
	scale_ = model_scale;
	pos_ = *firePos;
	vec_ = vec;
	isEnabled_ = true;
	fireObjectMoveVec_ = fireObjectMoveVec;

	// ベクトル方向の回転行列を作成
	rotMtx_ = MGetRotVec2(init_model_direction, *vec_);

	// ベクトル方向の回転行列からオイラー角を出力
	MATRIX rotEffectMtx = MGetRotVec2(init_effect_direction, *vec_);
	bool isGimbalLock = false;
	VECTOR effectRot = MathUtil::ToEulerAngles(rotEffectMtx, isGimbalLock);

	// レーザーのエフェクトの再生
	auto& effectManager = Effekseer3DEffectManager::GetInstance();
	effectManager.PlayEffectFollow(laserEffectHandle_, EffectID::normal_laser, firePos_, effect_scale, 1.0f, effectRot);

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
}

void NormalLazer::Update()
{
	collisionAndEffectDifferenceTimer_.Update(1);
	if (collisionAndEffectDifferenceTimer_.IsTimeOut())
	{
		if (!isRefrect_)
		{
			pos_ = VAdd(pos_, *fireObjectMoveVec_);
		}

		pos_ = VAdd(pos_, actualVec_);
	}
	else
	{
		actualVec_ = VScale(*vec_, move_speed);
		pos_ = *firePos_;

		// ベクトル方向の回転行列をモデルに設定
		rotMtx_ = MGetRotVec2(init_model_direction, *vec_);

		// ベクトル方向の回転行列からオイラー角を出力
		MATRIX rotEffectMtx = MGetRotVec2(init_effect_direction, *vec_);
		bool isGimbalLock = false;
		VECTOR effectRot = MathUtil::ToEulerAngles(rotEffectMtx, isGimbalLock);

		auto& effectManager = Effekseer3DEffectManager::GetInstance();
		effectManager.SetEffectRot(laserEffectHandle_, effectRot);
	}

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

void NormalLazer::Refrect(const VECTOR pos, const VECTOR norm)
{
	// レーザーのエフェクトをストップ
	auto& effectManager = Effekseer3DEffectManager::GetInstance();
	effectManager.DeleteEffect(laserEffectHandle_);

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
	actualVec_ = VAdd(*vec_, normVec);
	actualVec_ = VScale(actualVec_, 48.0f);
#else
	vec_ = VScale(vec_, -1);
#endif

	// ベクトル方向の回転行列をモデルに設定
	MATRIX rotModelMtx = MGetRotVec2(init_model_direction, actualVec_);
	MV1SetRotationMatrix(pModel_->GetModelHandle(), rotModelMtx);

	// ベクトル方向の回転行列からオイラー角を出力
	MATRIX rotEffectMtx = MGetRotVec2(init_effect_direction, actualVec_);
	bool isGimbalLock = false;
	VECTOR effectRot = MathUtil::ToEulerAngles(rotEffectMtx, isGimbalLock);

	// レーザーのエフェクトの再生
	effectManager.PlayEffectFollow(laserEffectHandle_, EffectID::refrect_laser, &effectPos_, effect_scale, 1.0f, effectRot);

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