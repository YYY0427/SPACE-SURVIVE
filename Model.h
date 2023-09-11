#pragma once
#include <DxLib.h>

/// <summary>
/// モデルの管理クラス
/// </summary>
class Model
{
public:
	// コンストラクタ
	Model(const char* fileName);	// ファイル名を指定してロードを行う
	Model(int orgModel);			// 指定されたハンドルのモデルをDuplicateして生成する

	// デストラクタ
	virtual ~Model();

	// 当たり判定設定
	void SetUseCollision(bool isUse, bool isNeedUpdate = true);

	// 更新
	void Update();

	// 描画
	void Draw();

	// アニメーションを設定する(ぱっと切り替える)
	void SetAnimation(int animNo, bool isLoop, bool isForceChange);

	// アニメーションを変化させる(数フレームかけて切り替える)
	void ChangeAnimation(int animNo, bool isLoop, bool isForceChange, int changeFrame);

	// 現在のアニメーションが終了しているかどうかを取得する(Loopアニメの場合は取得できません falseを返す)
	bool IsAnimEnd();

	// ゲッター
	int GetModelHandle() const { return modelHandle_; }		// モデルのハンドルの取得
	int GetColFrameIndex() const { return m_colFrameIndex; }	// 当たり判定に使用するフレームインデックスを取得する

	// セッター
	void SetPos(VECTOR pos);		// 表示位置の設定
	void SetRot(VECTOR rot);		// 回転状態の設定
	void SetScale(VECTOR scale);	// 拡大値の設定

private:
	// アニメーション情報
	struct AnimData
	{
		int animNo;			// アニメーション番号
		int attachNo;		// アタッチ番号
		float totalTime;	// アニメーションの総再生時間
		bool isLoop;		// アニメーションがループするか
	};

private:
	// アニメーション情報のクリア
	void ClearAnimData(AnimData& anim);

	// アニメーションの更新
	void UpdateAnim(AnimData anim, float dt = 1.0f);

	// 現在のアニメーション切り替わり情報からアニメーションのブレンド率を設定する
	void UpdateAnimBlendRate();
private:
	// モデルのハンドル
	int modelHandle_;

	// 当たり判定情報を使用する
	bool m_isUseCollision;

	// 当たり判定情報を毎フレーム更新する
	bool m_isUpdateColision;

	// 当たり判定として使用するフレームのインデックス
	int m_colFrameIndex;

	// アニメーションのアタッチ番号
	AnimData animPrev_;	// 変更前アニメーション情報
	AnimData animNext_;	// 変更後アニメーション情報

	// アニメーションの切り替え情報
	int animChangeFrame_;			// 現在の切り替えフレーム数
	int animChangeFrameTotal_;		// 切り替えにかける総フレーム数
};