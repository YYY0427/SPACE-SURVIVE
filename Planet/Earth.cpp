#include "Earth.h"
#include "../Util/Model.h"

Earth::Earth(int handle, UnityGameObject data)
{
	pos_ = data.pos;
	rot_ = data.rot;

	pModel_ = std::make_shared<Model>(handle);
	pModel_->SetUseCollision(true);
	pModel_->SetScale(VGet(data.scale.x, data.scale.y, data.scale.z));
	/*pModel_->SetScale(VGet(0.1f, 0.1f, 0.1f));*/
	pModel_->SetRot(rot_);
	pModel_->SetPos(pos_);
	pModel_->Update(); 
}

Earth::~Earth()
{
}

void Earth::Update()
{
	rot_.y += 0.001f;

	// 向いている方向の設定
	pModel_->SetRot(rot_);

	// アニメーションと当たり判定の更新
	pModel_->Update();

}

void Earth::Draw()
{
	pModel_->Draw();
	DrawSphere3D(pos_, 20, 8, 0xff0000, 0xff0000, true);
}