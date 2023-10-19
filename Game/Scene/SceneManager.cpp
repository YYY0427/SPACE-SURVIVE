#include "SceneManager.h"
#include "SceneBase.h"
#include "../Util/Effekseer3DEffectManager.h"

// �S�ẴV�[���̍폜�ƃV�[���̐؂�ւ�
void SceneManager::PopAllSceneAndChangeScene(SceneBase* scene)
{
	// ���ׂẴV�[���̍폜
	for (auto& scene : scene_)
	{
		// �V�[���̍폜
		delete scene;
	}
	// ���ׂĔz����폜
	scene_.clear();

	// ���̃V�[���̒ǉ�
	PushScene(scene);
}

// �V�[���̐؂�ւ�
void SceneManager::ChangeScene(SceneBase* scene)
{
	// �V�[���X�^�b�N����ł͂Ȃ�������
	if (!scene_.empty())
	{
		// ���݂̃V�[���̍폜
		delete scene_.front();		

		// �폜�����V�[���̔z����폜
		scene_.pop_front();			
	}
	// ���̃V�[���̒ǉ�
	PushScene(scene);
}

// ���݂̃V�[���̏�ɃV�[����ς�(�|�[�Y)
void SceneManager::PushScene(SceneBase* scene)
{
	// �V�[���̒ǉ�
	scene_.push_front(scene);
}

// ��ԏ�̃V�[�����폜����
void SceneManager::PopScene()
{
	// �V�[����0�ɂȂ�Ȃ��悤�ɂ���
	if (scene_.size() > 1)
	{
		// ���݂̃V�[���̍폜
		delete scene_.front();

		// �폜�����V�[���̔z����폜
		scene_.pop_front();
	}
}

// �Q�[�����I�����邩�ǂ����̎擾
bool SceneManager::GetIsGameEnd() const
{
	return isGameEnd_;
}

// �Q�[�����I�����邩�ǂ����̐ݒ�
void SceneManager::SetIsGameEnd(bool isGameEnd)
{
	isGameEnd_ = isGameEnd;
}

// �V�[���̍X�V
void SceneManager::Update()
{
	scene_.front()->Update();

	if (static_cast<int>(scene_.size()) == 1)
	{
		Effekseer3DEffectManager::GetInstance().Update();
	}
}

// �e�V�[���̕`��
void SceneManager::Draw()
{
	for (int i = static_cast<int>(scene_.size() - 1); 0 <= i; i--)
	{
		scene_[i]->Draw();
	}
	if (static_cast<int>(scene_.size()) == 1)
	{
		Effekseer3DEffectManager::GetInstance().Draw();
	}
}