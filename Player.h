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
	bool GameOverUpdate();

	// 描画
	void Draw();

	// ゲッター
	VECTOR GetPos();		// 位置情報
	bool GetIsBoost();		// ブースト状態か
	float GetSlowRate();	// スロモーションのレート

	// セッター
	void SetCamera(std::shared_ptr<Camera> pCamera);
private:
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

	// 左スティックが入力されたか
	bool isInput_;
};