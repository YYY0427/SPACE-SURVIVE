#include "Player.h"
#include "Shield.h"
#include "Util/InputState.h"
#include "Util/Model.h"
#include "Util//Effekseer3DEffectManager.h"
#include "Util/Debug.h"
#include "Util/Range.h"
#include "UIManager.h"
#include "common.h"
#include <string>

namespace
{
	// プレイヤーモデルのファイルのパス
	const std::string model_file_path = "Data/Model/Player.mv1";

	// プレイヤーの移動量
	constexpr VECTOR player_vec_up = { 0, 1, 0 };
	constexpr VECTOR player_vec_down = { 0, -1, 0 };
	constexpr VECTOR player_vec_right = { 1, 0, 0 };
	constexpr VECTOR player_vec_left = { -1, 0, 0 };

	// 初期位置
	constexpr VECTOR init_pos = { 0, 0, 0 };

	// モデルの回転率
	constexpr VECTOR init_rot = { 0, 0, 0 };

	// モデルの拡大率
	constexpr VECTOR model_scale = { 0.5f, 0.5f, 0.5f };

	// プレイヤーの通常移動速度
	constexpr float move_normal_speed = 1.5f;

	// エネルギーゲージ
	constexpr float energy_gauge_total_amount = 1000.0f;

	// エネルギーの回復量
	constexpr float energy_recovery_amount = 10.0f;

	// プレイヤーの当たり判定の半径
	constexpr float model_collision_radius = 30.0f;

	// 無敵時間のフレーム数
	constexpr int ultimate_frames = 120;

	// 最大HP
	constexpr int max_hp = 3;

	// 何フレーム前まで位置情報を保存するか
	constexpr int log_frame = 10;

	// プレイヤーの横幅
	constexpr float player_width = 50.0f;

	// プレイヤーの縦幅
	constexpr float player_height = 50.0f;
}

//  コンストラクタ
Player::Player(std::shared_ptr<UIManager> pUIManager) :
	moveVec_(VGet(0.0f, 0.0f, 0.0f)),
	hp_(max_hp),
	ultimateTimer_(0),
	isInput_(false),
	moveSpeed_(move_normal_speed),
	energyGauge_(energy_gauge_total_amount),
	playerDeadEffectHandle_(-1),
	isPlayGameOverEffect_(false),
	boostEffectScale_({ 20.0f, 20.0f, 20.0f }),
	boostEffectSpeed_(1.0f),
	waitTimer_(300),
	pUIManager_(pUIManager)
{
	pos_ = init_pos;
	rot_ = init_rot;

	// プレイヤーモデルのインスタンスの生成
	pModel_ = std::make_shared<Model>(model_file_path.c_str());

	// シールドのインスタンスの生成
	pShield_ = std::make_shared<Shield>(*this);

	// モデルの拡大率の設定
	pModel_->SetScale(model_scale);

	// 回転率の設定
	pModel_->SetRot(VGet(rot_.x, rot_.y, rot_.z));

	// 位置情報の設定
	pModel_->SetPos(pos_);

	// アニメーションと当たり判定の更新
	pModel_->Update();

	// ブーストエフェクトの再生
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

	// 左スティックの入力情報の取得
	int up = InputState::IsPadStick(PadLR::LEFT, PadStickInputType::UP);
	int down = InputState::IsPadStick(PadLR::LEFT, PadStickInputType::DOWN);
	int left = InputState::IsPadStick(PadLR::LEFT, PadStickInputType::LEFT);
	int right = InputState::IsPadStick(PadLR::LEFT, PadStickInputType::RIGHT);

	VECTOR moveUp = VTransform(player_vec_up, MGetRotY(cameraYaw));
	VECTOR moveDown = VTransform(player_vec_down, MGetRotY(cameraYaw));
	VECTOR moveRight = VTransform(player_vec_right, MGetRotY(cameraYaw));
	VECTOR moveLeft = VTransform(player_vec_left, MGetRotY(cameraYaw));

	// エネルギーの処理
	EnergyProcess();

	// 移動情報の初期化
	isInput_ = false;
	moveVec_ = VGet(0, 0, 0);
	VECTOR moveVecX = VGet(0, 0, 0);
	VECTOR moveVecY {0, 0, 0};

	// スティックが入力されていたら移動ベクトルにスティックが傾いている方向のベクトルを代入
	// スティックの傾きぐわいによってベクトルが大きくなる
	if (up > 0)
	{
		moveVecY = VScale(moveUp, up);
		isInput_ = true;
	}
	if (left > 0)
	{
		moveVecX = VScale(moveLeft, left);
		isInput_ = true;
	}
	if (down > 0)
	{
		moveVecY = VScale(moveDown, down);
		isInput_ = true;
	}
	if (right > 0)
	{
		moveVecX = VScale(moveRight, right);
		isInput_ = true;
	}

	// スティックが入力されている場合のみ移動
	if (isInput_)
	{
		// プレイヤーから見てx方向とz方向のベクトルを足して移動ベクトルを作成する
		moveVec_ = VAdd(moveVecY, moveVecX);

		// プレイヤーのスピードを掛ける
		moveVec_ = VScale(moveVec_, moveSpeed_);

		// 作成した移動ベクトルで座標の移動
		VECTOR tempPos = VAdd(pos_, moveVec_);

		// ワールド座標をスクリーン座標に変換
		VECTOR screenPos = ConvWorldPosToScreenPos(tempPos);

		if (screenPos.x > common::screen_width - player_width)
		{
			screenPos.x = common::screen_width - player_width;
			VECTOR worldPos = ConvScreenPosToWorldPos(screenPos);
			pos_.x = worldPos.x;
			pos_.y = worldPos.y;
		}
		else if (screenPos.x < 0 + player_width)
		{
			screenPos.x = 0 + player_width;
			VECTOR worldPos = ConvScreenPosToWorldPos(screenPos);
			pos_.x = worldPos.x;
			pos_.y = worldPos.y;
		}
		else if (screenPos.y > common::screen_height - player_height)
		{
			screenPos.y = common::screen_height - player_height;
			VECTOR worldPos = ConvScreenPosToWorldPos(screenPos);
			pos_.x = worldPos.x;
			pos_.y = worldPos.y;

		}
		else if (screenPos.y < 0 + player_height)
		{
			screenPos.y = 0 + player_height;
			VECTOR worldPos = ConvScreenPosToWorldPos(screenPos);
			pos_.x = worldPos.x;
			pos_.y = worldPos.y;
		}
		else
		{
			pos_ = tempPos;
		}
	}

	// ログに追加
	posLogTable_.push_front(pos_);

	// 保存可能な数を超えていたら末尾から消す
	if (posLogTable_.size() > log_frame)
	{
		posLogTable_.pop_back();
	}

	// 無敵時間のタイマーの更新
	// 0以下にはならない
	ultimateTimer_ = (std::max)(--ultimateTimer_, 0);

	// 移動ベクトルの大きさからプレイヤーの傾き具合を算出
	// X軸回転は進んでいるように見せるように常に30度を足す
	float rotX = 30.0f * DX_PI_F / 180.0f;
	rot_ = { rotX + moveVec_.z * 0.01f, 0.0f, -moveVec_.x * 0.01f };

	// エフェクトの設定
	effectManager.SetEffectRot(boostEffectHandle_, { rot_.x + DX_PI_F, rot_.y, -rot_.z});
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

void Player::GameOverUpdate()
{
	// UIを格納
	pUIManager_->StoreUI();

	waitTimer_.Update(1);
	if (waitTimer_.IsTimeOut() && !isPlayGameOverEffect_)
	{
		isPlayGameOverEffect_ = true;
		auto& effectManager = Effekseer3DEffectManager::GetInstance();
		effectManager.DeleteEffect(boostEffectHandle_);
		effectManager.PlayEffectFollow(playerDeadEffectHandle_, EffectID::player_dead, &pos_, { 50.0f, 50.0f, 50.0f }, 0.5f);
	}
	else if(!waitTimer_.IsTimeOut())
	{
		moveVec_ = { 0.0f, -0.1f, 1.0f };
		moveVec_ = VNorm(moveVec_);
		moveVec_ = VScale(moveVec_, 2.0f);

		// 作成した移動ベクトルで座標の移動
		pos_ = VAdd(pos_, moveVec_);
	}

	// 位置座標の設定
	pModel_->SetPos(pos_);

	// 向いている方向の設定
	pModel_->SetRot({ VSize(moveVec_), 0.0f, VSize(moveVec_) });

	// アニメーションと当たり判定の更新
	pModel_->Update();
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
	Debug::Log("playerPos", ConvWorldPosToScreenPos(pos_));
	Debug::Log("energyGauge", energyGauge_);
	Debug::Log("プレイヤーHP", hp_);

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

void Player::DrawUI()
{
	pShield_->DrawUI();
}

// ダメージ処理
void Player::OnDamage()
{
	// HPを減らす
	hp_--;
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

std::shared_ptr<Shield> Player::GetShield() const
{
	return pShield_;
}

std::deque<VECTOR> Player::GetPosLogTable() const
{
	return posLogTable_;
}
