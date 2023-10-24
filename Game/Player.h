#pragma once
#include <DxLib.h>
#include <memory>
#include "Util/DataReaderFromUnity.h"
#include "Util/Timer.h"
#include <deque>

class Model;
class Camera;
class Shield;

// プレイヤークラス
class Player
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="data"></param>
	Player(UnityGameObject data);

	// デストラクタ
	virtual ~Player();

	// 通常の更新
	void Update();

	/// <summary>
	/// 岩との衝突時の更新
	/// </summary>
	/// <returns>処理が終了したか true : 処理終了, false : 処理途中</returns>
	bool CollisionRockUpdate();

	// エネルギーの処理
	void EnergyProcess();

	// 描画
	void Draw();

	/// <summary>
	/// プレイヤーの落下処理 
	/// </summary>
	/// <param name="fallSpeed">落下スピード</param>
	void Fall(float fallSpeed);

	// プレイヤーのリスポーン処理
	void Respawn(VECTOR restartPos);

	// プレイヤーのダメージ処理
	void OnDamage();

	void A();

	/// </summary>
	/// プレイヤーの高さが落下死亡判定の高さより小さくなったか
	/// </summary>
	/// <returns>true : 小さくなった, false : 小さくなってない</returns>
	bool IsDeathJudgHeight() const;

	/// <summary>
	/// プレイヤーが無敵時間中か
	/// </summary>
	/// <returns>true : 無敵時間中, false : 無敵時間中じゃない</returns>
	bool IsUltimate() const;

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

	// カメラクラスのポインタの設定
	void SetCameraPointer(std::shared_ptr<Camera> pCamera);

	float GetCameraFar() const;
private:
	// ポインタ
	std::shared_ptr<Model> pModel_;
	std::shared_ptr<Camera> pCamera_;
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

	// 命
	int hp_;

	// 無敵時間
	int ultimateTimer_;

	// プレイヤーの移動速度
	float moveSpeed_;

	// エネルギーゲージ
	float energyGauge_;

	// 移動ベクトルを反転したか
	bool isReverseMoveVec_;

	// 左スティックが入力されたか
	bool isInput_;

	// ゲームオーバーエフェクトを再生したか
	bool isPlayGameOverEffect_;
};