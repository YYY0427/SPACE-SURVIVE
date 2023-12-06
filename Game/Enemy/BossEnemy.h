#pragma once
#include "EnemyBase.h"

class HpBar;
class Flash;
class Triangle;
class ScreenEffect;

class BossEnemy : public EnemyBase
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="modelHandle"></param>
	/// <param name="pPlayer"></param>
	/// <param name="pLazerManager"></param>
	BossEnemy(int modelHandle, std::shared_ptr<Player> pPlayer, std::shared_ptr<LaserManager> pLazerManager, std::shared_ptr<ScreenEffect> pScreenEffect);

	// デストラクタ
	~BossEnemy();

	// 更新
	void Update() override;

	// 描画
	void Draw() override;		// モデル、その他
	void DrawUI() override;		// UI

	/// <summary>
	/// ダメージ
	/// </summary>
	/// <param name="damage">受けたダメージ</param>
	/// <param name="pos">ダメージを受けた位置</param>
	void OnDamage(int damage, VECTOR pos) override;

private:
	// ステートの初期化
	void InitState() override;

	// 移動処理の初期化
	void InitMove();

	void Move();
	void CubeLaserAttack();

	// ステートの管理
	void StateManager();

private:
	// Entar
	void EntarEntry();
	void EntarIdle();
	void EntarMove();
	void EntarDamage();
	void EntarDied();
	void EntarStopCubeLaserAttack();
	void EntarStopNormalLaserAttack();
	void EntarMoveCubeLaserAttack();
	void EntarMoveNormalLaserAttack();

	// Update
	void UpdateEntry();
	void UpdateIdle();
	void UpdateMove();
	void UpdateDamage();
	void UpdateDied();
	void UpdateStopCubeLaserAttack();
	void UpdateStopNormalLaserAttack();
	void UpdateMoveCubeLaserAttack();
	void UpdateMoveNormalLaserAttack();

	// Exit
	void ExitEntry();
	void ExitIdle();
	void ExitMove();
	void ExitDamage();
	void ExitDied();
	void ExitStopCubeLaserAttack();
	void ExitStopNormalLaserAttack();
	void ExitMoveCubeLaserAttack();
	void ExitMoveNormalLaserAttack();

private:
	enum class State
	{
		ENTRY,
		IDLE,
		MOVE,
		DAMAGE,
		DEID,

		STOP_ATTACK_CUBE_LASER,
		STOP_ATTACK_NORMAL_LASER,
		MOVE_ATTACK_CUBE_LASER,
		MOVE_ATTACK_NORMAL_LASER,
	};

private:
	struct DieEffect
	{
		int effectHandle;
		VECTOR pos;
		VECTOR vec;
		float scale;
	};

private:
	// ステートマシン(ステートを管理する)
	StateMachine<State> stateMachine_;

	std::shared_ptr<ScreenEffect> pScreenEffect_;

	std::unique_ptr<Triangle> pTriangle_;
	std::unique_ptr<Flash> pFlash_;

	// HPバー
	std::unique_ptr<HpBar> pHpBar_;

	// 攻撃のステートを保存するテーブル
	std::vector<State> attackStateTable_;

	// 死亡時のエフェクト
	std::list<DieEffect> dieEffectTable_;

	int attackState_;

	int hitEffectHandle_;

	// キューブレーザーの移動速度
	float cubeLaserSpeed_;

	bool isDraw_;

	// 通常レーザーの発射位置
	VECTOR normalLaserFirePos_;

	// 移動
	std::vector<VECTOR> movePointTable_;	// 移動する地点のテーブル
	int movePoint_;		// 現在の移動地点
	VECTOR goalPos_;	// 目標の地点の位置
	bool isGoal_;		// 目標地点に到達したか
	bool isMoveEnd_;	// 全ての地点に到達して移動を終了したか
};