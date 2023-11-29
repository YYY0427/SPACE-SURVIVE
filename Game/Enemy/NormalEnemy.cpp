#include "NormalEnemy.h"
#include "../Util/Model.h"
#include "../Laser/LazerManager.h"
#include "../Player.h"
#include "../common.h"
#include "../Vector2.h"
#include "../Util/Debug.h"
#include "../Util/InputState.h"

namespace
{
	// レーザーの発射位置のフレーム
	constexpr int lazer_fire_frame_pos = 37;

	// 当たり判定の半径
	constexpr float collision_radius = 48.0f;

	constexpr int anim_frame = 1;

	constexpr float move_error_range = 10.0f;

	constexpr VECTOR model_scale = { 0.7f, 0.7f, 0.7f };
	constexpr VECTOR init_model_direction = { 0, 0, -1 };
}

NormalEnemy::NormalEnemy(
	int modelHandle,
	std::shared_ptr<Player> pPlayer,
	std::shared_ptr<LazerManager> pLazerManager,
	VECTOR initPos,
	std::vector<EnemyAIData> normalEnemyGoalPosTable)
{
	pPlayer_ = pPlayer;
	pLaserManager_ = pLazerManager;
	opacity_ = 1.0f;
	pos_ = initPos;
	normalEnemyGoalPosTable_ = normalEnemyGoalPosTable;
	collisionRadius_ = collision_radius;
	movePoint_ = 0;
	isGoal_ = false;

	// ステートの初期化
	InitState();

	// ステートの設定
	stateMachine_.SetState(State::NORMAL);

	// インスタンス生成
	pModel_ = std::make_unique<Model>(modelHandle);

	// モデルの設定
	pModel_->SetOpacity(opacity_);	// 不透明度
	pModel_->SetPos(pos_);
	pModel_->SetRot(rot_);
	pModel_->SetScale(model_scale);
	pModel_->ChangeAnimation(anim_frame, true, false, 8);
	pModel_->Update();
}

NormalEnemy::~NormalEnemy()
{
}

void NormalEnemy::InitState()
{
	stateMachine_.AddState(
		State::NORMAL,
		[this]() { this->EntarNormal(); },
		[this]() { this->UpdateNormal(); },
		[this]() { this->ExitNormal(); });
	stateMachine_.AddState(
		State::IDLE,
		[this]() { this->EntarIdle(); },
		[this]() { this->UpdateIdle(); },
		[this]() { this->ExitIdle(); });
	stateMachine_.AddState(
		State::SHOT,
		[this]() { this->EntarShot(); },
		[this]() { this->UpdateShot(); },
		[this]() { this->ExitShot(); });
	stateMachine_.AddState(
		State::DEID,
		[this]() { this->EntarDeid(); },
		[this]() { this->UpdateDeid(); },
		[this]() { this->ExitDeid(); });
	stateMachine_.AddState(
		State::DEBUG,
		[this]() { this->EntarDebug(); },
		[this]() { this->UpdateDebug(); },
		[this]() { this->ExitDebug(); });
}

void NormalEnemy::Update()
{
	stateMachine_.Update();

	if (InputState::IsTriggered(InputType::NORMAL_ENEMY_DEBUG))
	{
		stateMachine_.SetState(State::DEBUG);
	}

	// レーザーの発射位置のフレーム座標の取得
	// 追従させるために毎フレーム取得
	firePos_ = MV1GetFramePosition(pModel_->GetModelHandle(), lazer_fire_frame_pos);

	// プレイヤーに向かうベクトルを作成
	toTargetVec_ = VSub(pPlayer_->GetPosLogTable().back(), firePos_);
	toTargetVec_ = VNorm(toTargetVec_);

	VECTOR vec = VSub(pPlayer_->GetPos(), pos_);
	vec = VNorm(vec);

	MATRIX rotMtx = MGetRotVec2(init_model_direction, vec);

	pos_ = VAdd(pos_, moveVec_);

	// モデルの設定
	pModel_->SetOpacity(opacity_);	// 不透明度
	pModel_->SetRotMtx(rotMtx);		// 回転行列
	pModel_->SetPos(pos_);			// 位置
	pModel_->Update();				// アニメーションの更新

	Debug::Log("NormalEnemyPos", pos_);
}

void NormalEnemy::Draw()
{
	pModel_->Draw();

#ifdef _DEBUG
//	DrawSphere3D(MV1GetFramePosition(pModel_->GetModelHandle(), lazer_fire_frame_pos), 10, 8, 0xff0000, 0xff0000, 0xff0000);
//	DrawSphere3D(pos_, collisionRadius_, 8, 0xff0000, 0xff0000, 0xff0000);
#endif
}

void NormalEnemy::EntarIdle()
{
	moveVec_ = { 0, 0, 0 };

	utilTimerTable_["idle"] = idleTime_;
}

void NormalEnemy::EntarNormal()
{
	auto itr = normalEnemyGoalPosTable_.begin();
	std::advance(itr, movePoint_);

	goalPos_ = itr->goalPos;
	float speed = itr->speed;

	VECTOR vec = VSub(goalPos_, pos_);
	vec = VNorm(vec);
	vec = VScale(vec, speed);
	moveVec_ = vec;
}

void NormalEnemy::EntarShot()
{
	moveVec_ = { 0, 0, 0 };

	// レーザーを発射
	pLaserManager_->Create(LaserType::NORMAL, &firePos_, &toTargetVec_, 1.5f);
}

void NormalEnemy::EntarDeid()
{
}

void NormalEnemy::EntarDebug()
{
}

void NormalEnemy::UpdateIdle()
{
	SinWave(50, 5);

	utilTimerTable_["idle"].Update(1);
	if (utilTimerTable_["idle"].IsTimeOut())
	{
		stateMachine_.SetState(State::NORMAL);
	}
}

void NormalEnemy::UpdateNormal()
{
	// 目標の地点に到達したら目標地点を進める
	if (pos_.x <= goalPos_.x + move_error_range && goalPos_.x - move_error_range <= pos_.x &&
		pos_.y <= goalPos_.y + move_error_range && goalPos_.y - move_error_range <= pos_.y &&
		pos_.z <= goalPos_.z + move_error_range && goalPos_.z - move_error_range <= pos_.z &&
		!isGoal_)
	{
		isGoal_ = true;

		movePoint_++;

		auto itr = normalEnemyGoalPosTable_.begin();
		std::advance(itr, movePoint_);

		if (normalEnemyGoalPosTable_.size() <= movePoint_)
		{
			isEnabled_ = false;
		}
		else
		{
			itr--;

			// 
			idleTime_ = itr->idleTime;

			// 到着した地点でのショット発射するフラグが立っていたらショットのステートに変更
			if (itr->isShot)
			{
				stateMachine_.SetState(State::SHOT);
			}
			else
			{
				stateMachine_.SetState(State::IDLE);
			}
		}
	}
	else
	{
		isGoal_ = false;
	}
}

void NormalEnemy::UpdateShot()
{
	stateMachine_.SetState(State::IDLE);
}

void NormalEnemy::UpdateDeid()
{
}

void NormalEnemy::UpdateDebug()
{
	moveVec_ = { 0, 0, 0 };
	if (InputState::IsPadStick(PadLR::RIGHT, PadStickInputType::UP))
	{
		moveVec_.y += 10.0f;
	}
	if (InputState::IsPadStick(PadLR::RIGHT, PadStickInputType::DOWN))
	{
		moveVec_.y -= 10.0f;
	}
	if (InputState::IsPadStick(PadLR::RIGHT, PadStickInputType::LEFT))
	{
		moveVec_.x -= 10.0f;
	}
	if (InputState::IsPadStick(PadLR::RIGHT, PadStickInputType::RIGHT))
	{
		moveVec_.x += 10.0f;
	}
	if (InputState::IsPadTrigger(PadLR::LEFT))
	{
		moveVec_.z -= 10.0f;
	}
	if (InputState::IsPadTrigger(PadLR::RIGHT))
	{
		moveVec_.z += 10.0f;
	}
}

void NormalEnemy::ExitIdle()
{
	utilTimerTable_.clear();
}

void NormalEnemy::ExitNormal()
{
}

void NormalEnemy::ExitShot()
{
}

void NormalEnemy::ExitDeid()
{
}

void NormalEnemy::ExitDebug()
{
}
