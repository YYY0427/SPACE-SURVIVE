#pragma once
#include <DxLib.h>
#include <memory>
#include "Util/Timer.h"
#include <deque>

class Model;
class Shield;

// プレイヤークラス
class Player
{
public:
	// コンストラクタ
	Player();

	// デストラクタ
	~Player();

	// 更新
	void Update(float cameraYaw);
	void GameOverUpdate();

	// 描画
	void Draw();
	void DrawUI();

	// エネルギーの処理
	void EnergyProcess();

	// プレイヤーのダメージ処理
	void OnDamage();

	/// <summary>
	/// プレイヤーが生きているか
	/// </summary>
	/// <returns>true : 生きている, false : 死んでいる</returns>
	bool IsLive() const;

	// 位置情報の取得
	VECTOR GetPos() const;			

	// プレイヤーの当たり判定の半径の取得
	float GetCollsionRadius() const;	

	// プレイヤーモデルのハンドルの取得
	int GetModelHandle() const; 

	// シールドのインスタンスの取得
	std::shared_ptr<Shield> GetShield() const;

	// 決められたフレームの数だけ位置情報を保存するテーブルの取得
	std::deque<VECTOR> GetPosLogTable() const;

private:
	// ポインタ
	std::shared_ptr<Model> pModel_;
	std::shared_ptr<Shield> pShield_;

	// 決められたフレーム前まで位置情報を保存しているテーブル
	std::deque<VECTOR> posLogTable_;

	// エフェクトハンドル
	int boostEffectHandle_;
	int playerDeadEffectHandle_;

	// 位置情報
	VECTOR pos_;
	
	// 回転情報
	VECTOR rot_;

	// 移動ベクトル
	VECTOR moveVec_;

	// ブーストエフェクトの拡大率
	VECTOR boostEffectScale_;

	// ブーストエフェクトの再生速度
	float boostEffectSpeed_;

	// 命
	int hp_;

	// 無敵時間
	int ultimateTimer_;

	// プレイヤーの移動速度
	float moveSpeed_;

	// エネルギーゲージ
	float energyGauge_;

	// 左スティックが入力されたか
	bool isInput_;

	// ゲームオーバーエフェクトを再生したか
	bool isPlayGameOverEffect_;

	Timer<int> waitTimer_;
};