#pragma once
#include <DxLib.h>
#include <memory>

class Model;
class Camera;

/// <summary>
/// プレイヤークラス
/// </summary>
class Player
{
public:
	// コンストラクタ
	Player();

	// デストラクタ
	virtual ~Player();

	// 更新
	void Update();

	/// <summary>
	/// ゲームオーバー時のプレイヤーの更新
	/// </summary>
	/// <returns>エフェクトを再生し終えたか</returns>
	bool GameOverUpdate();

	// 描画
	void Draw();

	// 位置情報の取得
	VECTOR GetPos();			

	/// <summary>
	/// ブースト状態かの取得
	/// </summary>
	/// <returns>true : ブースト状態、false : 通常状態</returns>
	bool GetIsBoost();			

	// スローモーションのレートの取得
	float GetSlowRate();		

	// プレイヤーの当たり判定の半径の取得
	float GetCollsionRadius();	

	// カメラクラスのポインタの設定
	void SetCameraPointer(std::shared_ptr<Camera> pCamera);
private:
	// ポインタ
	std::shared_ptr<Model> pModel_;
	std::shared_ptr<Camera> pCamera_;

	// 位置情報
	VECTOR pos_;

	// プレイヤーの移動速度
	float moveSpeed_;

	// エネルギーゲージ
	float energyGauge_;

	// スローモーションのレート
	float slowRate_;

	// スローモーション状態かどうか
	bool isSlow_;

	// ブースト状態かどうか
	bool isBoost_;

	// 左スティックが入力されたか
	bool isInput_;

	// エフェクトを一回だけ再生
	bool isEffectFirst_;
};