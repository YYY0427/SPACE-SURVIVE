#include "SceneManager.h"
#include "Scene.h"

/// <summary>
/// �S�ẴV�[���̍폜�ƃV�[���̐؂�ւ�
/// ��Ƀ|�[�Y����̃V�[���̐؂�ւ����s���Ƃ��Ɏg��
/// </summary>
/// <param name="scene"></param>
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

/// <summary>
/// �V�[���̐؂�ւ����s��
/// </summary>
/// <param name="scene">�؂�ւ��������V�[���̃A�h���X</param>
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

/// <summary>
/// �V�[������ɐς�(�|�[�Y)
/// Update�Ŏ��s�����̂͏�ɂ܂ꂽ�V�[��
/// </summary>
/// <param name="scene"></param>
void SceneManager::PushScene(Scene* scene)
{
	// �V�[���̒ǉ�
	scene_.push_front(scene);		
}

/// <summary>
/// ��ԏ�̃V�[�����폜����
/// </summary>
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

/// <summary>
/// �e�V�[����Update���s��
/// </summary>
void SceneManager::Update()
{
	scene_.front()->Update();
}

/// <summary>
/// �e�V�[���̕`��
/// </summary>
void SceneManager::Draw()
{
	for (int i = static_cast<int>(scene_.size() - 1); i >= 0; --i)
	{
		scene_[i]->Draw();
	}
}