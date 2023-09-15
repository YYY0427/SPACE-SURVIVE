#include "Player.h"
#include "Util/InputState.h"
#include "Util/Model.h"
#include <string>

namespace
{
	// モデルのファイルのパス
	string model_file_path = "Data/Model/UFO.mv1";
}

/// <summary>
///  コンストラクタ
/// </summary>
Player::Player() : 
	pos_(VGet(0.0f, 0.0f, 0.0f))
{
	pModel_ = make_shared<Model>(model_file_path.c_str());
	pModel_->SetScale(VGet(10.0f, 10.0f, 10.0f));
}

/// <summary>
///  デストラクタ
/// </summary>
Player::~Player()
{
}

/// <summary>
/// 更新
/// </summary>
void Player::Update()
{
	int up = InputState::IsXInputStick(XInputType::LEFT, XInputTypeStick::UP);
	int down = InputState::IsXInputStick(XInputType::LEFT, XInputTypeStick::DOWN);
	int left = InputState::IsXInputStick(XInputType::LEFT, XInputTypeStick::LEFT);
	int right = InputState::IsXInputStick(XInputType::LEFT, XInputTypeStick::RIGHT);

	pos_.z -= up;
	pos_.z += down;
	pos_.x -= left;
	pos_.x += right;

	// 位置座標の設定
	pModel_->SetPos(pos_);

	// 向いている方向の設定
//	pModel_->SetRot(VGet(0.0f, pCamera_->GetCameraYaw(), 0.0f));

	// アニメーションを進める
	pModel_->Update();
}

/// <summary>
/// 描画
/// </summary>
void Player::Draw()
{
	pModel_->Draw();
}