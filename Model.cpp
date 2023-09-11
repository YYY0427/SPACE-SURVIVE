#include "Model.h"
#include <cassert>

namespace
{
	// 当たり判定として使用するフレームの名前
	const char* const collision_frame_name = "Collision";
}

/// <summary>
/// 
/// </summary>
/// <param name="fileName"></param>
Model::Model(const char* fileName) :
	m_isUseCollision(false),
	m_isUpdateColision(false),
	m_colFrameIndex(-1),
	animChangeFrame_(0),
	animChangeFrameTotal_(0)
{
	modelHandle_ = MV1LoadModel(fileName);

	// モデルのロードに失敗した
	assert(modelHandle_ != -1);

	// アニメーション情報のクリア
	ClearAnimData(animPrev_);
	ClearAnimData(animNext_);
}

/// <summary>
/// 指定されたハンドルのモデルをDuplicateして生成する
/// </summary>
/// <param name="orgModel">モデルハンドル</param>
Model::Model(int orgModel) :
	m_isUseCollision(false),
	m_isUpdateColision(false),
	m_colFrameIndex(-1),
	animChangeFrame_(0),
	animChangeFrameTotal_(0)
{
	// ロードに失敗したモデルがコピー元として指定されている
	assert(modelHandle_ != -1);
	modelHandle_ = MV1DuplicateModel(orgModel);

	// アニメーション情報のクリア
	ClearAnimData(animPrev_);
	ClearAnimData(animNext_);
}

/// <summary>
/// デストラクタ
/// </summary>
Model::~Model()
{
	if (m_isUpdateColision)
	{
		// 当たり判定情報を使用していたら破棄する
		MV1TerminateCollInfo(modelHandle_, -1);
		m_isUseCollision = false;
	}
	MV1DeleteModel(modelHandle_);
}

/// <summary>
/// 当たり判定設定
/// </summary>
/// <param name="isUse">当たり判定をしようするか</param>
/// <param name="isNeedUpdate">更新が必要かどうか</param>
void Model::SetUseCollision(bool isUse, bool isNeedUpdate)
{
	// 当たり判定を使わないのに当たり判定の更新を毎フレームおこないたい場合をはじく
	assert(isUse || !isNeedUpdate);

	// 当たり判定使用情報に変化があった場合のみ更新する
	if (m_isUpdateColision != isUse)
	{
		if (isUse)
		{
			// 使わない→使う
			// 当たり判定用フレームを検索
			m_colFrameIndex = MV1SearchFrame(modelHandle_, collision_frame_name);

			if (m_colFrameIndex < 0)	// 見つからなかった or エラー
			{
				m_colFrameIndex = -1;
			}
			MV1SetupCollInfo(modelHandle_, m_colFrameIndex, 8, 8, 8);
		}
		else
		{
			// 使う→使わない
			MV1TerminateCollInfo(modelHandle_, -1);
		}
	}

	// 保存
	m_isUseCollision = isUse;
	m_isUpdateColision = isNeedUpdate;
}

/// <summary>
/// 更新
/// </summary>
void Model::Update()
{
	UpdateAnim(animPrev_);
	UpdateAnim(animNext_);

	// 指定フレームかけてアニメーションを変更する
	animChangeFrame_++;
	if (animChangeFrame_ > animChangeFrameTotal_)
	{
		animChangeFrame_ = animChangeFrameTotal_;
	}

	// アニメーションのブレンド率を設定する
	UpdateAnimBlendRate();

	// 当たり判定データの更新
	if (m_isUseCollision && m_isUpdateColision)
	{
		MV1RefreshCollInfo(modelHandle_, m_colFrameIndex);
	}
}

/// <summary>
///  描画
/// </summary>
void Model::Draw()
{
	MV1DrawModel(modelHandle_);
}

void Model::SetPos(VECTOR pos)
{
	MV1SetPosition(modelHandle_, pos);
}

void Model::SetRot(VECTOR rot)
{
	MV1SetRotationXYZ(modelHandle_, rot);
}

void Model::SetScale(VECTOR scale)
{
	MV1SetScale(modelHandle_, scale);
}

/// <summary>
/// アニメーションを設定する(ぱっと切り替える)
/// </summary>
/// <param name="animNo">変更先アニメーション番号</param>
/// <param name="isLoop">アニメーションをループさせるか</param>
/// <param name="isForceChange">すでに指定されたアニメが再生されている場合も変更するか</param>
void Model::SetAnimation(int animNo, bool isLoop, bool isForceChange)
{
	if (!isForceChange)
	{
		// すでに再生されているアニメは再生しない
		if (animNext_.animNo == animNo)	return;
	}

	// 以前のアニメーションが残っていれば終了
	if (animPrev_.attachNo != -1)
	{
		MV1DetachAnim(modelHandle_, animPrev_.attachNo);
		ClearAnimData(animPrev_);
	}
	if (animNext_.attachNo != -1)
	{
		MV1DetachAnim(modelHandle_, animNext_.attachNo);
		ClearAnimData(animNext_);
	}
	// 新しくアニメーションを設定
	animNext_.animNo = animNo;
	animNext_.attachNo = MV1AttachAnim(modelHandle_, animNo, -1, false);
	animNext_.totalTime = MV1GetAttachAnimTotalTime(modelHandle_, animNext_.attachNo);
	animNext_.isLoop = isLoop;

	animChangeFrameTotal_ = 1;
	animChangeFrame_ = 1;
}

/// <summary>
/// アニメーションを変化させる(数フレームかけて切り替える)
/// </summary>
/// <param name="animNo">アニメーション番号</param>
/// <param name="isLoop">アニメーションをループさせるか</param>
/// <param name="isForceChange">すでに指定されたアニメが再生されている場合も変更するか</param>
/// <param name="changeFrame">何フレームかけてアニメーションを変更させるか</param>
void Model::ChangeAnimation(int animNo, bool isLoop, bool isForceChange, int changeFrame)
{
	if (!isForceChange)
	{
		// すでに再生されているアニメは再生しない
		if (animNext_.animNo == animNo)	return;
	}

	// 以前のアニメーションが残っていれば終了
	if (animPrev_.attachNo != -1)
	{
		MV1DetachAnim(modelHandle_, animPrev_.attachNo);
		ClearAnimData(animPrev_);
	}

	// 現在再生しているアニメーションを古いアニメとする
	animPrev_ = animNext_;

	// 新しくアニメーションを設定
	animNext_.animNo = animNo;
	animNext_.attachNo = MV1AttachAnim(modelHandle_, animNo, -1, false);
	animNext_.totalTime = MV1GetAttachAnimTotalTime(modelHandle_, animNext_.attachNo);
	animNext_.isLoop = isLoop;

	// 変更にかけるフレーム数を覚えておく
	animChangeFrameTotal_ = changeFrame;
	animChangeFrame_ = 0;

	// アニメーションのブレンド率を設定する
	UpdateAnimBlendRate();
}

/// <summary>
/// // 現在のアニメーションが終了しているかどうかを取得する(Loopアニメの場合は取得できません falseを返す)
/// </summary>
/// <returns>true : 終了 、 false : 再生中</returns>
bool Model::IsAnimEnd()
{
	// Looppアニメの場合は常にfalseを返す
	if (animNext_.isLoop) return false;

	float time = MV1GetAttachAnimTime(modelHandle_, animNext_.attachNo);

	if (time >= animNext_.totalTime) return true;

	return false;
}

/// <summary>
/// アニメーションデータのクリア
/// </summary>
/// <param name="anim">アニメーションのアタッチ番号</param>
void Model::ClearAnimData(AnimData& anim)
{
	anim.animNo = -1;
	anim.attachNo = -1;
	anim.totalTime = 0.0f;
	anim.isLoop = false;
}

/// <summary>
/// アニメーションの更新
/// </summary>
/// <param name="anim"></param>
/// <param name="dt"></param>
void Model::UpdateAnim(AnimData anim, float dt)
{
	// アニメーションが設定されていない場合は何もしない
	if (anim.attachNo == -1) return;

	// アニメーションの更新
	float time = MV1GetAttachAnimTime(modelHandle_, anim.attachNo);
	time += dt;
	if (time > anim.totalTime)
	{
		if (anim.isLoop)
		{
			// アニメーションのループ
			time -= anim.totalTime;
		}
		else
		{
			// 最後の状態で止まる
			time = anim.totalTime;
		}
	}
	// アタッチしているアニメーションの再生時間を設定する
	MV1SetAttachAnimTime(modelHandle_, anim.attachNo, time);
}

/// <summary>
//  現在のアニメーション切り替わり情報からアニメーションのブレンド率を設定する
/// </summary>
void Model::UpdateAnimBlendRate()
{
	// アニメーション変化のフレーム数に応じたブレンド率を設定する
	float rate = static_cast<float> (animChangeFrame_) / static_cast<float>(animChangeFrameTotal_);
	if (rate > 1.0f)	rate = 1.0f;
	MV1SetAttachAnimBlendRate(modelHandle_, animPrev_.attachNo, 1.0f - rate);
	MV1SetAttachAnimBlendRate(modelHandle_, animNext_.attachNo, rate);
}