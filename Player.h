#pragma once
#include <DxLib.h>
#include <memory>

class Model;
class Camera;

// プレイヤークラス
class Player
{
public:
	// コンストラクタ
	Player();

	// デストラクタ
	virtual ~Player();

	// 通常の更新
	void Update();

	/// <summary>
	/// ゲームオーバー時の更新
	/// </summary>
	/// <returns>エフェクトを再生し終えたか</returns>
	bool GameOverUpdate();

	// ブーストの処理
	void BoostProcess();

	// スローモーションの処理
	void SlowProcess();

	// エネルギーの処理
	void EnergyProcess();

	// 描画
	void Draw();

	// 位置情報の取得
	VECTOR GetPos() const;			

	/// <summary>
	/// ブースト状態かの取得
	/// </summary>
	/// <returns>true : ブースト状態、false : 通常状態</returns>
	bool GetIsBoost() const;			

	// スローモーションのレートの取得
	float GetSlowRate() const;		

	// プレイヤーの当たり判定の半径の取得
	float GetCollsionRadius() const;	

	// カメラクラスのポインタの設定
	void SetCameraPointer(std::shared_ptr<Camera> pCamera);
private:
	// ポインタ
	std::shared_ptr<Model> pModel_;
	std::shared_ptr<Camera> pCamera_;

	// 位置情報
	VECTOR pos_;
	
	// 移動ベクトル
	VECTOR moveVec_;

	// プレイヤーの移動速度
	float moveSpeed_;

	// エネルギーゲージ
	float energyGauge_;

	// スローモーションのレート
	float slowRate_;

	// 移動ベクトルを反転したか
	bool isReverseMoveVec_;

	// スローモーション状態かどうか
	bool isSlow_;

	// ブースト状態かどうか
	bool isBoost_;

	// 左スティックが入力されたか
	bool isInput_;

	// ゲームオーバーエフェクトを再生したか
	bool isPlayGameOverEffect_;
};