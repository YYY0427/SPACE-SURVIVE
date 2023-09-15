#pragma once
#include <deque>

class Scene;

/// <summary>
/// �V�[���Ď��N���X
/// </summary>
class SceneManager
{
public:
	// �V�[���̐؂�ւ�
	void ChangeScene(Scene* scene);

	// �V�[������ɐς�(�|�[�Y)
	void PushScene(Scene* scene);

	/// ��ԏ�̃V�[�����폜
	void PopScene();

	// �e�V�[���̍X�V
	void Update();

	// �e�V�[���̕`��
	void Draw();

private:
	// �����s���̃V�[����؂�ւ��Ă��������̂�	
	// �Q�Ƃł͂Ȃ��|�C���^�Ƃ��Đ錾
	std::deque<Scene*> scene_;
};

