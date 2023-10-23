#include "Earth.h"
#include "../Util/Model.h"

Earth::Earth(int handle, UnityGameObject data)
{
	pos_ = data.pos;
	rot_ = data.rot;

	pModel_ = std::make_shared<Model>(handle);
	pModel_->SetUseCollision(true);
	pModel_->SetScale(VGet(data.scale.x, data.scale.y, data.scale.z));
	pModel_->SetRot(rot_);
	pModel_->SetPos(pos_);
	pModel_->Update(); 
}

Earth::~Earth()
{
}

void Earth::Update()
{
	// �����Ă�������̐ݒ�
	pModel_->SetRot(rot_);

	// �A�j���[�V�����Ɠ����蔻��̍X�V
	pModel_->Update();
}

void Earth::Draw()
{
	pModel_->Draw();
}