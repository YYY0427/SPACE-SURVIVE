#include "Player.h"
#include "Shield.h"
#include "Util/InputState.h"
#include "Util/Model.h"
#include "Util//Effekseer3DEffectManager.h"
#include "Util/Debug.h"
#include "Util/Range.h"
#include <string>

namespace
{
	// プレイヤーモデルのファイルのパス
	std::string model_file_path = "Data/Model/MV1/";
	std::string model_file_extension = ".mv1";

	// プレイヤーの移動量
	constexpr VECTOR player_vec_up = { 0, 0, 1 };
	constexpr VECTOR player_vec_down = { 0, 0, -1 };
	constexpr VECTOR player_vec_right = { 1, 0, 0 };
	constexpr VECTOR player_vec_left = { -1, 0, 0 };

	// モデルの拡大率
	constexpr float model_scale = 0.5f;

	// プレイヤーの通常移動速度
	constexpr float move_normal_speed = 1.5f;

	// エネルギーゲージ
	constexpr float energy_gauge_total_amount = 1000.0f;

	// エネルギーの回復量
	constexpr float energy_recovery_amount = 10.0f;

	// プレイヤーの当たり判定の半径
	constexpr float model_collision_radius = 30.0f;

	// 落下死亡判定の高さ
	constexpr float death_judg_height = -1000.0f;

	// 無敵時間のフレーム数
	constexpr int ultimate_frames = 120;

	// 残機数
	constexpr int max_hp = 2;

	// 何フレーム前まで位置情報を保存するか
	constexpr int log_frame = 2;
}

//  コンストラクタ
Player::Player(UnityGameObject data) :
	pos_(data.pos),
	rot_(data.rot),
	moveVec_(VGet(0.0f, 0.0f, 0.0f)),
	hp_(max_hp),
	ultimateTimer_(0),
	isInput_(false),
	moveSpeed_(move_normal_speed),
	energyGauge_(energy_gauge_total_amount),
	playerDeadEffectHandle_(-1),
	isPlayGameOverEffect_(false),
	isReverseMoveVec_(false),
	boostEffectScale_(20.0f),
	boostEffectSpeed_(1.0f)
{
	// プレイヤーモデルのインスタンスの生成
	std::string path = model_file_path + data.name + model_file_extension;
	pModel_ = std::make_shared<Model>(path.c_str());

	// シールドのインスタンスの生成
	pShield_ = std::make_shared<Shield>(*this);

	// モデルの拡大率の設定
//	pModel_->SetScale(data.scale);
	pModel_->SetScale(VGet(model_scale, model_scale, model_scale));

	// 回転率の設定
	pModel_->SetRot(VGet(rot_.x, rot_.y, rot_.z));

	// 位置情報の設定
	pModel_->SetPos(pos_);

	// アニメーションと当たり判定の更新
	pModel_->Update();

	Effekseer3DEffectManager::GetInstance().PlayEffectLoopAndFollow(boostEffectHandle_, EffectID::player_boost, &pos_, boostEffectScale_, boostEffectSpeed_, { rot_.x, 0.0f, 0.0f });
}

//  デストラクタ
Player::~Player()
{

}

// 更新
void Player::Update(float cameraYaw)
{
	auto& effectManager = Effekseer3DEffectManager::GetInstance();

	posLogTable_.push_front(pos_);
	if (log_frame < static_cast<int>(posLogTable_.size()))
	{
		posLogTable_.pop_back();
	}

	// 左スティックの入力情報の取得
	int up = InputState::IsPadStick(PadLR::LEFT, PadStickInputType::UP);
	int down = InputState::IsPadStick(PadLR::LEFT, PadStickInputType::DOWN);
	int left = InputState::IsPadStick(PadLR::LEFT, PadStickInputType::LEFT);
	int right = InputState::IsPadStick(PadLR::LEFT, PadStickInputType::RIGHT);

	// カメラが向いている方向からベクトル変換して移動情報作成
#if false
	VECTOR moveForward = VScale(VNorm(VSub(pCamera_->GetTarget(), pCamera_->GetPos())), -1);
	VECTOR moveBack = VScale(moveForward, -1);
#else 
	VECTOR moveForward = VTransform(player_vec_up, MGetRotY(cameraYaw));
	VECTOR moveBack = VTransform(player_vec_down, MGetRotY(cameraYaw));
#endif
	VECTOR moveRight = VTransform(player_vec_right, MGetRotY(cameraYaw));
	VECTOR moveLeft = VTransform(player_vec_left, MGetRotY(cameraYaw));

	// エネルギーの処理
	EnergyProcess();

	// 移動情報の初期化
	isInput_ = false;
	moveVec_ = VGet(0, 0, 0);
	VECTOR moveVecX = VGet(0, 0, 0);
	VECTOR moveVecZ = VGet(0, 0, 0);

	// スティックが入力されていたら移動ベクトルにスティックが傾いている方向のベクトルを代入
	// スティックの傾きぐわいによってベクトルが大きくなる
	if (up > 0)
	{
		moveVecZ = VScale(moveForward, up);
		isInput_ = true;

		// ブーストエフェクトの拡大率を大きくする
		// 特定の大きさより大きくならない
		boostEffectScale_++;
		boostEffectScale_ = (std::fmin)(boostEffectScale_, 40.0f);
	}
	if (left > 0)
	{
		moveVecX = VScale(moveLeft, left);
		isInput_ = true;
	}
	if (down > 0)
	{
		moveVecZ = VScale(moveBack, down);
		isInput_ = true;

		// ブーストエフェクトの拡大率を小さくする
		// 特定の大きさより小さくならない
		boostEffectScale_--;
		boostEffectScale_ = (std::fmax)(boostEffectScale_, 20.0f);
	}
	if (right > 0)
	{
		moveVecX = VScale(moveRight, right);
		isInput_ = true;
	}
	// 上下のパッドスティック入力がなかった場合
	if (up <= 0 && down <= 0)
	{
		// ブーストエフェクトの拡大率をデフォルトの大きさに戻す
		if (boostEffectScale_ >= 30.0f)
		{
			boostEffectScale_--;
			boostEffectScale_ = (std::fmax)(boostEffectScale_, 30.0f);
		}
		else
		{
			boostEffectScale_++;
			boostEffectScale_ = (std::fmin)(boostEffectScale_, 30.0f);
		}
	}

	if (InputState::IsPadTrigger(PadLR::LEFT))
	{
		pos_.y += 5;
	}
	if (InputState::IsPadTrigger(PadLR::RIGHT))
	{
		pos_.y -= 5;
	}

	// スティックが入力されている場合のみ移動
	if (isInput_)
	{
		// プレイヤーから見てx方向とz方向のベクトルを足して移動ベクトルを作成する
		moveVec_ = VAdd(moveVecZ, moveVecX);

		// プレイヤーのスピードを掛ける
		moveVec_ = VScale(moveVec_, moveSpeed_);

		// 作成した移動ベクトルで座標の移動
		pos_ = VAdd(pos_, moveVec_);
	}

	// 無敵時間のタイマーの更新
	// 0以下にはならない
	ultimateTimer_ = (std::max)(--ultimateTimer_, 0);

	// 移動ベクトルの大きさからプレイヤーの傾き具合を算出
	// X軸回転は進んでいるように見せるように常に30度を足す
	float rotX = 30.0f * DX_PI_F / 180.0f;
	rot_ = { rotX + moveVec_.z * 0.01f, 0.0f, -moveVec_.x * 0.01f };

	effectManager.SetEffectRot(boostEffectHandle_, { rot_.x + (180.0f * DX_PI_F / 180.0f), rot_.y, -rot_.z});
	effectManager.SetEffectScale(boostEffectHandle_, boostEffectScale_);
	effectManager.SetEffectSpeed(boostEffectHandle_, boostEffectSpeed_);

	// 位置座標の設定
	pModel_->SetPos(pos_);

	// 向いている方向の設定
	pModel_->SetRot(rot_);

	// アニメーションを進める
	pModel_->Update();

	// シールドの更新
	pShield_->Update();
}

// 岩との衝突時の更新
bool Player::CollisionRockUpdate()
{
	// ブースト時のエフェクトの再生のストップ
	Effekseer3DEffectManager::GetInstance().DeleteEffect(boostEffectHandle_);
	
	// 移動ベクトルを反転していなかったら反転
	// 既に反転していたら反転しない
	if (!isReverseMoveVec_)
	{
		isReverseMoveVec_ = true;
		moveVec_ = VScale(moveVec_, -1.0f);
	}

	// ベクトルを徐々に小さくする
	moveVec_ = VScale(moveVec_, 0.96f);

	// 作成した移動ベクトルで座標の移動
	pos_ = VAdd(pos_, moveVec_);

	// ベクトルが特定の大きさよりも小さくなったらゲームオーバーエフェクト再生
	if(VSize(moveVec_) <= 1.0f)
	{
		// ダメージ処理
		OnDamage();

		if (hp_ <= 0)
		{
			// ゲームオーバーエフェクトを再生してなかったら再生
			// 既に再生していたら再生しない
			if (!isPlayGameOverEffect_)
			{
				isPlayGameOverEffect_ = true;
				Effekseer3DEffectManager::GetInstance().PlayEffect(playerDeadEffectHandle_, EffectID::player_dead, pos_, 50.0f, 0.5f);
			}
			// エフェクトを再生し終えたらtrueを返す
			if (!Effekseer3DEffectManager::GetInstance().IsPlayingEffect(playerDeadEffectHandle_) && isPlayGameOverEffect_)
			{
				// 処理終了
				return true;
			}
		}
		else
		{
			// 初期化
			isReverseMoveVec_ = false;

			// 処理終了
			return true;
		}
	}
	
	// 位置座標の設定
	pModel_->SetPos(pos_);

	// 向いている方向の設定
	pModel_->SetRot(VGet(VSize(moveVec_) / 5.0f, 0.0f, VSize(moveVec_) / 10.0f));

	// アニメーションと当たり判定の更新
	pModel_->Update();

	// 処理が途中なのでfalseを返す
	return false;
}

// エネルギー処理
void Player::EnergyProcess()
{
	energyGauge_ += energy_recovery_amount;

	// エネルギー上限に達したら回復を止める
	if (energyGauge_ > energy_gauge_total_amount)
	{
		energyGauge_ = energy_gauge_total_amount;
	}
}

// 描画
void Player::Draw()
{
	Debug::Log("playerPos", pos_);
	Debug::Log("energyGauge", energyGauge_);
	Debug::Log("残機", hp_);

	// 無敵時間の点滅
	if (IsUltimate())
	{
		if ((ultimateTimer_ / 5) % 2 == 0)
		{
			return;
		}
	}

	// プレイヤーモデルの描画
	if (!isPlayGameOverEffect_)
	{
		pModel_->Draw();
#ifdef _DEBUG
		DrawSphere3D(pos_, model_collision_radius, 8, 0xff0000, 0xff0000, false);
#endif 
	}
	pShield_->Draw();
}

// プレイヤーの落下処理
void Player::Fall(float fallSpeed)
{
	// 落下
	pos_.y -= fallSpeed;
}

// プレイヤーのリスポーン処理
void Player::Respawn(VECTOR restartPos)
{
	// Y軸の値を大きくする
	// そのままだと道にめり込むため
	restartPos = VGet(restartPos.x, restartPos.y + 200.0f, restartPos.z);

	// リスポーン
	pos_ = restartPos;
}

// ダメージ処理
void Player::OnDamage()
{
	// 無敵時間中はダメージを受けない
	if (IsUltimate()) return;

	// HPを減らす
	hp_--;

	// 無敵時間の設定
	ultimateTimer_ = ultimate_frames;
}

void Player::A()
{
	pos_ = posLogTable_[0];
}

// プレイヤーの高さが落下死亡判定の高さより小さくなったか
bool Player::IsDeathJudgHeight() const
{
	return (pos_.y < death_judg_height) ? true : false;
}

// プレイヤーが無敵時間中か
bool Player::IsUltimate() const
{
	return (ultimateTimer_ > 0) ? true : false;
}

bool Player::IsLive() const
{
	return (hp_ > 0 ) ? true : false;
}

// 位置情報の取得
VECTOR Player::GetPos() const
{
	return pos_;
}

// プレイヤーの当たり判定の半径の取得
float Player::GetCollsionRadius() const
{
	return model_collision_radius;
}

// プレイヤーモデルのハンドルの取得
int Player::GetModelHandle() const
{
	return pModel_->GetModelHandle();
}

VECTOR Player::GetMoveVec() const
{
	return moveVec_;
}
