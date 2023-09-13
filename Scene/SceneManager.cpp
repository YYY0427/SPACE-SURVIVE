#include "SceneManager.h"
#include "Scene.h"

/// <summary>
/// �V�[���̐؂�ւ����s��
/// </summary>
/// <param name="scene">�؂�ւ��������V�[���̃A�h���X</param>
void SceneManager::ChangeScene(Scene* scene)
{
	//�V�[���X�^�b�N����ł͂Ȃ�������...
	if (!scene_.empty())
	{
		delete scene_.front();		// top�͂Ă��؂�̗v�f��Ԃ��Ă�
		scene_.pop_front();			// 1���� -> �ӂ���0�ɂȂ�
	}
	scene_.push_front(scene);		// 1������
}

/// <summary>
/// �V�[������ɐς�(�|�[�Y)
/// Update�Ŏ��s�����̂͏�ɂ܂ꂽ�V�[���̂�
/// </summary>
/// <param name="scene"></param>
void SceneManager::PushScene(Scene* scene)
{
	scene_.push_front(scene);		// 1������
}

/// <summary>
/// ��ԏ�̃V�[�����폜����
/// </summary>
void SceneManager::PopScene()
{
	//�|�b�v�̌��ʁA�V�[����0�ɂȂ�Ȃ��悤�ɂ���
	if (scene_.size() > 1)
	{
		delete scene_.front();
		scene_.pop_front();
	}
}

/// <summary>
/// �e�V�[����Update���s��
/// </summary>
void SceneManager::Update(const InputState& input)
{
	scene_.front()->Update(input);
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