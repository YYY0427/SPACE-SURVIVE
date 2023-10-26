#include "Shield.h"
#include "Player.h"
#include "Util/Effekseer3DEffectManager.h"
#include "Util/Model.h"
#include "Util/InputState.h"
#include "Util/Debug.h"
#include "Util/Geometry.h"
#include "Util/Range.h"
#include "Util/Timer.h"
#include <string>

namespace
{
	const std::string model_file_path = "Data/Model/MV1/Shield.mv1";
	constexpr float model_scale = 100.0f;
	constexpr float effect_scale = 80.0f;
	constexpr int max_enerugy_gage = 500;
}

Shield::Shield(Player& player) :
	player_(player),
	isInput_(false),
	effectHandle_(-1),
	enerugyGage_(max_enerugy_gage)
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
	// 初期化
	auto& effectManager = Effekseer3DEffectManager::GetInstance();
	effectManager.DeleteEffect(effectHandle_);
	isInput_ = false;
	const Range<int> enerugyGageRange(0, max_enerugy_gage);

	// 右スティックの入力情報の取得
	int up = InputState::IsPadStick(PadLR::RIGHT, PadStickInputType::UP);
	int down = InputState::IsPadStick(PadLR::RIGHT, PadStickInputType::DOWN);
	int right = InputState::IsPadStick(PadLR::RIGHT, PadStickInputType::RIGHT);
	int left = InputState::IsPadStick(PadLR::RIGHT, PadStickInputType::LEFT);

	// 右スティックが入力されたか
	if (up > 5 || down > 5 || right > 5 || left > 5)
	{
		isInput_ = true;
	}

	// スティックの入力情報からベクトルを作成
	int z = (up + -down) * 10;
	int x = (right + -left) * 10;
	VECTOR vec = { x, 0.0f, z };

	// ベクトルから角度を作成
	float rot = -atan2f(z, x);
	Debug::Log("角度", rot * 180.0f / DX_PI_F);

	// プレイヤーの平行移動行列の取得
	MATRIX playerMtx = MGetTranslate(player_.GetPos());

	// シールドの相対位置とプレイヤーの平行行列から位置情報を作成
	pos_ = VTransform(vec, playerMtx);

	if (isInput_)
	{
		if (enerugyGage_ > 0)
		{
			// シールドを出している間は常にエネルギーゲージを減らす
			enerugyGage_--;

			// シールドエフェクトの再生
			effectManager.PlayEffect(effectHandle_, EffectID::player_shield, { pos_.x, pos_.y - 100.0f, pos_.z }, effect_scale, 1.0f, { 0.0f, rot, 0.0f });
		}
	}
	else
	{
		enerugyGage_++;
	}

	enerugyGage_ = enerugyGageRange.Clamp(enerugyGage_);
	Debug::Log("エネルギーゲージ", enerugyGage_);

	pModel_->SetPos(pos_);
	pModel_->SetRot({0.0f, rot, 0.0f});
	pModel_->Update();
}

void Shield::Draw()
{
	if (isInput_ && enerugyGage_ > 0)
	{
		pModel_->Draw();
	}
}
