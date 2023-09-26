#include "SceneManager.h"
#include "Scene.h"

// �S�ẴV�[���̍폜�ƃV�[���̐؂�ւ�
void SceneManager::PopAllSceneAndChangeScene(Scene* scene)
{
	// ���ׂẴV�[���̍폜
	for (auto& scene : scene_)
	{
		delete scene_.front();
		scene_.pop_front(); 
	}

	// ���̃V�[���̒ǉ�
	scene_.push_front(scene);
}

// �V�[���̐؂�ւ�
void SceneManager::ChangeScene(Scene* scene)
{
	// �V�[���X�^�b�N����ł͂Ȃ�������
	if (!scene_.empty())
	{
		// ���݂̃V�[���̍폜
		delete scene_.front();		
		scene_.pop_front();			
	}
	// ���̃V�[���̒ǉ�
	scene_.push_front(scene);		
}

// ���݂̃V�[���̏�ɃV�[����ς�(�|�[�Y)
void SceneManager::PushScene(Scene* scene)
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
		// �V�[���̍폜
		delete scene_.front();
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
}

// �e�V�[���̕`��
void SceneManager::Draw()
{
	for (int i = static_cast<int>(scene_.size() - 1); 0 <= i; i--)
	{
		scene_[i]->Draw();
	}
}