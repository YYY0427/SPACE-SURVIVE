#include "Shield.h"
#include "Player.h"
#include "Util/Effekseer3DEffectManager.h"
#include "Util/Model.h"
#include "Util/InputState.h"
#include "Util/Debug.h"
#include "Util/Geometry.h"
#include <string>

namespace
{
	const std::string model_file_path = "Data/Model/MV1/Shield.mv1";
	constexpr float model_scale = 100.0f;
	constexpr float effect_scale = 80.0f;

}

Shield::Shield(Player& player) :
	player_(player),
	isShield_(false),
	effectHandle_(-1)
{
	pModel_ = std::make_shared<Model>(model_file_path);
	pModel_->SetPos(player_.GetPos());
	pModel_->SetScale({ model_scale , model_scale , model_scale });
}

Shield::~Shield()
{
}

void Shield::Update()
{
	auto& effectManager = Effekseer3DEffectManager::GetInstance();
	effectManager.DeleteEffect(effectHandle_);
	isShield_ = false;

	int up = InputState::IsPadStick(PadLR::RIGHT, PadStickInputType::UP);
	int down = InputState::IsPadStick(PadLR::RIGHT, PadStickInputType::DOWN);
	int right = InputState::IsPadStick(PadLR::RIGHT, PadStickInputType::RIGHT);
	int left = InputState::IsPadStick(PadLR::RIGHT, PadStickInputType::LEFT);

	// 入力されたか
	if (up > 5 || down > 5 || right > 5 || left > 5)
	{
		isShield_ = true;
	}

	int z = (-up + down) * 10;
	int x = (right + -left) * 10;
	VECTOR vec = { x, 0.0f, z };

	float rot = atan2f(z, x);
	Debug::Log("角度", rot * 180.0f / DX_PI_F);

	// プレイヤーの平行移動行列の取得
	MATRIX playerMtx = MGetTranslate(player_.GetPos());

	// シールドの相対位置とプレイヤーの平行行列から位置情報を作成
	pos_ = VTransform(vec, playerMtx);

	if (isShield_)
	{
		effectManager.PlayEffect(effectHandle_, EffectID::player_shield, { pos_.x, pos_.y - 100.0f, pos_.z }, effect_scale, 1.0f, {0.0f, rot, 0.0f});
	}

	pModel_->SetPos(pos_);
	pModel_->SetRot({0.0f, rot, 0.0f});
	pModel_->Update();
}

void Shield::Draw()
{
	if (isShield_)
	{
		pModel_->Draw();
	}
}
