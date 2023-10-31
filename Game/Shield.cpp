#include "Shield.h"
#include "Player.h"
#include "Util/Effekseer3DEffectManager.h"
#include "Image3D.h"
#include "Util/InputState.h"
#include "Util/Debug.h"
#include "Util/Geometry.h"
#include "Util/Range.h"
#include "Util/Timer.h"
#include <string>

namespace
{
	const std::string model_file_path = "Data/Image/Shield.png";
	constexpr float effect_scale = 80.0f;
	constexpr int max_enerugy_gage = 100000;
}

Shield::Shield(Player& player) :
	player_(player),
	isInput_(false),
	effectHandle_(-1),
	enerugyGage_(max_enerugy_gage)
{
	pShiled_ = std::make_shared<Image3D>(model_file_path);
	pShiled_->SetPos(player_.GetPos());
	pShiled_->SetImgWidth(100.0f);
	pShiled_->SetImgHeight(100.0f);
}

Shield::~Shield()
{
}

void Shield::Update()
{
	// ������
	auto& effectManager = Effekseer3DEffectManager::GetInstance();
	effectManager.DeleteEffect(effectHandle_);
	isInput_ = false;
	const Range<int> enerugyGageRange(0, max_enerugy_gage);

	// �E�X�e�B�b�N�̓��͏��̎擾
	int up = InputState::IsPadStick(PadLR::RIGHT, PadStickInputType::UP);
	int down = InputState::IsPadStick(PadLR::RIGHT, PadStickInputType::DOWN);
	int right = InputState::IsPadStick(PadLR::RIGHT, PadStickInputType::RIGHT);
	int left = InputState::IsPadStick(PadLR::RIGHT, PadStickInputType::LEFT);

	// �E�X�e�B�b�N�����͂��ꂽ��
	if (up > 5 || down > 5 || right > 5 || left > 5)
	{
		isInput_ = true;
	}

	// �X�e�B�b�N�̓��͏�񂩂�x�N�g�����쐬
	int z = (up + -down) * 10;
	int x = (right + -left) * 10;
	VECTOR vec = { x, 0.0f, z };

	// �x�N�g������p�x���쐬
	float rot = -atan2f(z, x);
	Debug::Log("�p�x", rot * 180.0f / DX_PI_F);

	// �v���C���[�̕��s�ړ��s��̎擾
	MATRIX playerMtx = MGetTranslate(player_.GetPos());

	// �V�[���h�̑��Έʒu�ƃv���C���[�̕��s�s�񂩂�ʒu�����쐬
	pos_ = VTransform(vec, playerMtx);

	if (isInput_)
	{
		if (enerugyGage_ > 0)
		{
			// �V�[���h���o���Ă���Ԃ͏�ɃG�l���M�[�Q�[�W�����炷
			enerugyGage_--;

			// �V�[���h�G�t�F�N�g�̍Đ�
			effectManager.PlayEffect(effectHandle_, EffectID::player_shield, { pos_.x, pos_.y - 100.0f, pos_.z }, effect_scale, 1.0f, { 0.0f, rot, 0.0f });
		}
	}
	else
	{
		enerugyGage_++;
	}

	enerugyGage_ = enerugyGageRange.Clamp(enerugyGage_);
	Debug::Log("�G�l���M�[�Q�[�W", enerugyGage_);

	pShiled_->SetPos(pos_);
	pShiled_->SetRot({0.0f, rot + (90.0f * DX_PI_F / 180.0f), 0.0f});
	pShiled_->Update();
}

void Shield::Draw()
{
	if (GetIsShield())
	{
		pShiled_->Draw();
	}
}

VECTOR Shield::GetPos() const
{
	return pos_;
}

bool Shield::GetIsShield() const
{
	return (enerugyGage_ > 0) && (isInput_);
}

std::array<VERTEX3D, 6> Shield::GetVertex() const
{
	return pShiled_->GetVertex();
}
