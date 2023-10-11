#include "Model.h"
#include <cassert>

namespace
{
	// 当たり判定として使用するフレームの名前
	const std::string collision_frame_name = "Collision";
}

// ファイル名を指定してロードを行う
Model::Model(std::string fileName) :
	isUseCollision_(false),
	isUpdateColision_(false),
	colFrameIndex_(-1),
	animChangeFrame_(0),
	animChangeFrameTotal_(0)
{
	// モデルのロード
	modelHandle_ = MV1LoadModel(fileName.c_str());

	// モデルのロードに失敗したしたら止める
	assert(modelHandle_ != -1);

	// アニメーション情報の初期化
	InitAnimData(animPrev_);
	InitAnimData(animNext_);
}

// 指定されたハンドルのモデルをコピーして生成する
Model::Model(int modelHandle) :
	isUseCollision_(false),
	isUpdateColision_(false),
	colFrameIndex_(-1),
	animChangeFrame_(0),
	animChangeFrameTotal_(0)
{
	// モデルのコピー
	modelHandle_ = MV1DuplicateModel(modelHandle);

	// モデルのコピーに失敗したら止める
	assert(modelHandle_ != -1);

	// アニメーション情報の初期化
	InitAnimData(animPrev_);
	InitAnimData(animNext_);
}

// デストラクタ
Model::~Model()
{
	// 当たり判定情報を使用していたら削除
	if (isUpdateColision_)
	{
		MV1TerminateCollInfo(modelHandle_, -1);
		isUseCollision_ = false;
	}

	// モデルの削除
	MV1DeleteModel(modelHandle_);
}

// 当たり判定の設定
void Model::SetUseCollision(bool isUse, bool isNeedUpdate)
{
	// 当たり判定を使わないのに当たり判定の更新を毎フレームおこないたい場合をはじく
	assert(isUse || !isNeedUpdate);

	// 当たり判定使用情報に変化があった場合のみ更新する
	if (isUpdateColision_ != isUse)
	{
		if (isUse)
		{
			// 使わない→使う
			// 当たり判定用フレームを検索
			colFrameIndex_ = MV1SearchFrame(modelHandle_, collision_frame_name.c_str());

			// 見つからなかった or エラー
			if (colFrameIndex_ < 0)	
			{
				colFrameIndex_ = -1;
			}

			// 当たり判定情報を構築する
			MV1SetupCollInfo(modelHandle_, colFrameIndex_, 8, 8, 8);
		}
		else
		{
			// 使う→使わない
			MV1TerminateCollInfo(modelHandle_, -1);
		}
	}

	// 保存
	isUseCollision_ = isUse;
	isUpdateColision_ = isNeedUpdate;
}

// 更新
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
	if (isUseCollision_ && isUpdateColision_)
	{
		MV1RefreshCollInfo(modelHandle_, colFrameIndex_);
	}
}

//  描画
void Model::Draw()
{
	MV1DrawModel(modelHandle_);
}

// モデルの表示位置の設定
void Model::SetPos(VECTOR pos)
{
	MV1SetPosition(modelHandle_, pos);
}

// モデルの回転状態の設定
void Model::SetRot(VECTOR rot)
{
	MV1SetRotationXYZ(modelHandle_, rot);
}

// モデルの拡大率の設定
void Model::SetScale(VECTOR scale)
{
	MV1SetScale(modelHandle_, scale);
}

// アニメーションを設定する(ぱっと切り替える)
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
		InitAnimData(animPrev_);
	}
	if (animNext_.attachNo != -1)
	{
		MV1DetachAnim(modelHandle_, animNext_.attachNo);
		InitAnimData(animNext_);
	}

	// 新しくアニメーションを設定
	animNext_.animNo = animNo;
	animNext_.attachNo = MV1AttachAnim(modelHandle_, animNo, -1, false);
	animNext_.totalTime = MV1GetAttachAnimTotalTime(modelHandle_, animNext_.attachNo);
	animNext_.isLoop = isLoop;

	animChangeFrameTotal_ = 1;
	animChangeFrame_ = 1;
}

// アニメーションを変化させる(数フレームかけて切り替える)
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
		InitAnimData(animPrev_);
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

// 現在のアニメーションが終了しているかどうかを取得する
bool Model::IsAnimEnd()
{
	// Looppアニメの場合はfalseを返す
	if (animNext_.isLoop)
	{
		return false;
	}

	// アタッチしているアニメーションの再生時間を取得する
	float time = MV1GetAttachAnimTime(modelHandle_, animNext_.attachNo);

	// アニメーションを再生した時間がアニメーションの総時間を超えていたら
	// アニメーションが終了しているのでtrueを返す
	if (time >= animNext_.totalTime)
	{
		return true;
	}
	return false;
}

// モデルのハンドルの取得
int Model::GetModelHandle() const
{
	return modelHandle_;		
}

// 当たり判定に使用するフレームインデックスを取得する
int Model::GetColFrameIndex() const
{
	return colFrameIndex_;
}

// アニメーションデータの初期化
void Model::InitAnimData(AnimData& anim)
{
	anim.animNo = -1;
	anim.attachNo = -1;
	anim.totalTime = 0.0f;
	anim.isLoop = false;
}

// アニメーションの更新
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

// 現在のアニメーション切り替わり情報からアニメーションのブレンド率を設定する
void Model::UpdateAnimBlendRate()
{
	// アニメーション変化のフレーム数に応じたブレンド率を設定する
	float rate = static_cast<float> (animChangeFrame_) / static_cast<float>(animChangeFrameTotal_);
	if (rate > 1.0f)
	{
		rate = 1.0f;
	}
	MV1SetAttachAnimBlendRate(modelHandle_, animPrev_.attachNo, 1.0f - rate);
	MV1SetAttachAnimBlendRate(modelHandle_, animNext_.attachNo, rate);
}