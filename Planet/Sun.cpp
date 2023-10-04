#include "Sun.h"
#include "../Util/Model.h"

Sun::Sun(int handle, UnityGameObject data)
{
//	pModel_ = std::make_shared<Model>(handle);

	pos_ = data.pos;
	rot_ = data.rot;

	pModel_ = std::make_shared<Model>(handle);
	pModel_->SetUseCollision(true);
	pModel_->SetScale(VGet(data.scale.x, data.scale.y, data.scale.z));
	pModel_->SetRot(rot_);
	pModel_->SetPos(pos_);
	pModel_->Update();
}

Sun::~Sun()
{
}

void Sun::Update()
{
	rot_.y += 0.001f;

	// 向いている方向の設定
	pModel_->SetRot(rot_);

	// アニメーションと当たり判定の更新
	pModel_->Update();
}

void Sun::Draw()
{
	pModel_->Draw();
}