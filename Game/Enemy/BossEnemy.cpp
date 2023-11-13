#include "BossEnemy.h"
#include "../Util/Model.h"
#include "../Util/Timer.h"
#include "../UI/HpBar.h"
#include "../Laser/LazerManager.h"
#include "../Player.h"
#include "../Util/Effekseer3DEffectManager.h"

namespace
{
	constexpr int hp_bar_side_space = 300;
	constexpr int hp_bar_start_y = 50;
	constexpr int hp_bar_height = 30;
	constexpr float aim_hp_speed = 0.09f;
	constexpr float max_hp = 100.0f;

	// レーザーの発射位置フレーム
	constexpr int lazer_fire_frame_pos = 1374;
}

BossEnemy::BossEnemy(int modelHandle, std::shared_ptr<Player> pPlayer, std::shared_ptr<LazerManager> pLazerManager, UnityGameObject data) 
{
	pPlayer_ = pPlayer;
	pLazerManager_ = pLazerManager;
	pModel_ = std::make_unique<Model>(modelHandle);
	pos_ = data.pos;
	rot_ = { data.rot.x, data.rot.y, data.rot.z };
	lazerFireIntervalTimer_ = 120;
	pModel_->SetScale(data.scale);
	lazerSpeed_ = 20.0f;

	hp_ = max_hp;
	pHpBar_ = std::make_unique<HpBar>(max_hp);
}

BossEnemy::~BossEnemy()
{
}

void BossEnemy::Update()
{
	lazerFireIntervalTimer_.Update(1);
	if (lazerFireIntervalTimer_.IsTimeOut())
	{
		// レーザーの発射位置のフレーム座標の取得
		VECTOR firePos = MV1GetFramePosition(pModel_->GetModelHandle(), lazer_fire_frame_pos);

		VECTOR vec = VSub(pPlayer_->GetPos(), pos_);
		vec = VNorm(vec);
		vec = VScale(vec, lazerSpeed_);
		pLazerManager_->Create(LazerType::CUBE, &firePos, &vec, {});
		lazerFireIntervalTimer_.Reset();
	}

	SinWave(500, 5);

	pHpBar_->Update(aim_hp_speed);

	pModel_->SetRot(rot_);
	pModel_->SetPos(pos_);
	pModel_->Update();
}

void BossEnemy::Draw()
{
	pModel_->Draw();

	pHpBar_->Draw(hp_bar_side_space, hp_bar_start_y, hp_bar_height);
}

void BossEnemy::OnDamage(int damage, VECTOR pos)
{
	hp_ -= damage;

	pHpBar_->OnDamage(hp_);

	Effekseer3DEffectManager::GetInstance().PlayEffect(
		onDamageEffectHandle_,
		EffectID::enemy_on_damage,
		pos,
		200.0f,
		0.5f);
}
