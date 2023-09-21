#pragma once
#include <deque>

// �v���g�^�C�v�錾
class Scene;

// �V�[���̊Ǘ��N���X
class SceneManager
{
public:
	/// <summary>
	/// �S�ẴV�[���̍폜�ƃV�[���̐؂�ւ�
	/// ��Ƀ|�[�Y����̃V�[���̐؂�ւ����s���Ƃ��Ɏg��
	/// </summary>
	/// <param name="scene">�؂�ւ������V�[���̃A�h���X</param>
	void PopAllSceneAndChangeScene(Scene* scene);

	/// <summary>
	/// �V�[���̐؂�ւ�
	/// </summary>
	/// <param name="scene">�؂�ւ������V�[���̃A�h���X</param>
	void ChangeScene(Scene* scene);

	/// <summary>
	/// ���݂̃V�[���̏�ɃV�[����ς�(�|�[�Y)
	/// Update�Ŏ��s�����̂͏�ɂ܂ꂽ�V�[��
	/// </summary>
	/// <param name="scene">��ɐς݂����V�[���̃A�h���X</param>
	void PushScene(Scene* scene);

	// ��ԏ�̃V�[�����폜
	// �|�[�Y��ʂ������Ƃ��Ɏg��
	void PopScene();

	// �V�[���̍X�V
	// ��ԏ�̃V�[�������X�V
	void Update();

	// �e�V�[���̕`��
	void Draw();

private:
	// �V�[��
	// �����s���̃V�[����؂�ւ��Ă��������̂�	
	// �Q�Ƃł͂Ȃ��|�C���^�Ƃ��Đ錾
	std::deque<Scene*> scene_;
};

