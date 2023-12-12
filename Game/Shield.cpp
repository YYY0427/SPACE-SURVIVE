#include "Shield.h"
#include "Player.h"
#include "Util/Effekseer3DEffectManager.h"
#include "Image3D.h"
#include "UI/EnergyGage.h"
#include "Util/InputState.h"
#include "Util/Debug.h"
#include "Util/Range.h"
#include "Util/Timer.h"
#include "Util/MathUtil.h"
#include <string>

namespace
{
	const std::string model_file_path = "Data/Image/Shield.png";
	constexpr VECTOR effect_scale = { 80.0f,  80.0f, 80.0f, };
	constexpr int max_enerugy_gage = 10000;
	constexpr float player_distance = 70.0f;
}

Shield::Shield(Player& player) :
	player_(player),
	isInput_(false),
	effectHandle_(-1),
	enerugyGage_(max_enerugy_gage)
{
	pShiled_ = std::make_shared<Image3D>(model_file_path);
	pEnergyGage_ = std::make_unique<EnergyGage>(max_enerugy_gage);
	pShiled_->SetPos(player_.GetPos());
	pShiled_->SetImgWidth(70.0f);
	pShiled_->SetImgHeight(70.0f);
}

Shield::~Shield()
{
}

void Shield::Update()
{
	// プレイヤーが生きているか
	if (player_.IsLive())
	{
		// 初期化
		auto& effectManager = Effekseer3DEffectManager::GetInstance();
		effectManager.DeleteEffect(effectHandle_);
		isInput_ = false;
		const Range<int> enerugyGageRange(0, max_enerugy_gage);

		if (InputState::IsPressed(InputType::SHIELD))
		{
			isInput_ = true;
		}

		// 右スティックの入力情報の取得
		int up = InputState::IsPadStick(PadLR::RIGHT, PadStickInputType::UP);
		int down = InputState::IsPadStick(PadLR::RIGHT, PadStickInputType::DOWN);
		int right = InputState::IsPadStick(PadLR::RIGHT, PadStickInputType::RIGHT);
		int left = InputState::IsPadStick(PadLR::RIGHT, PadStickInputType::LEFT);

		VECTOR testVec = { (right + -left) * 10.0f, (up + -down) * 10.0f, player_distance};

		// プレイヤーの平行移動行列の取得
		MATRIX playerMtx = MGetTranslate(player_.GetPos());

		// シールドの相対位置とプレイヤーの平行行列から位置情報を作成
		pos_ = VTransform(testVec, playerMtx);
	//	pos_ = { player_.GetPos().x, player_.GetPos().y, player_.GetPos().z + player_distance };

		// ベクトルから角度を求める
		bool isGimbalLock = false;
		rot_ = MathUtil::ToEulerAngles(MGetRotVec2(VGet(0, 0, 1), testVec), isGimbalLock);

		if (isInput_)
		{
			if (enerugyGage_ > 0)
			{
				// シールドを出している間は常にエネルギーゲージを減らす
				enerugyGage_--;

				// シールドエフェクトの再生
				effectManager.PlayEffect(effectHandle_, EffectID::player_shield, { pos_.x, pos_.y - 100.0f, pos_.z }, effect_scale, 1.0f, { rot_});
			}
		}
		else
		{
			enerugyGage_++;
		}

		enerugyGage_ = enerugyGageRange.Clamp(enerugyGage_);
		Debug::Log("エネルギーゲージ", enerugyGage_);

		pShiled_->SetPos(pos_);
		pShiled_->SetRot({ rot_ });
		pShiled_->Update();
	}
	else
	{
		auto& effectManager = Effekseer3DEffectManager::GetInstance();
		effectManager.DeleteEffect(effectHandle_);
	}
}

void Shield::Draw()
{
	if (IsShield() && player_.IsLive())
	{
#ifdef _DEBUG
		pShiled_->Draw();
#endif
	}
}

void Shield::DrawUI()
{
	VECTOR screenPlayerPos = ConvWorldPosToScreenPos(player_.GetPos());
	pEnergyGage_->Draw(screenPlayerPos.x, screenPlayerPos.y - 40, enerugyGage_);
}

VECTOR Shield::GetPos() const
{
	return pos_;
}

bool Shield::IsShield() const
{
	return (enerugyGage_ > 0) && (isInput_);
}

std::array<VERTEX3D, 6> Shield::GetVertex() const
{
	return pShiled_->GetVertex();
}
