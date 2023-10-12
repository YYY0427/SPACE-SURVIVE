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

	/// <summary>
	/// �Q�[�����I�����邩�ǂ����̎擾
	/// main.cpp��while���𔲂��邽�߂ɍ쐬
	/// </summary>
	/// <returns>true : �Q�[���I��, false : �I�����Ȃ�</returns>
	bool GetIsGameEnd() const;

	/// <summary>
	/// �Q�[�����I�����邩�ǂ����̐ݒ�
	/// main.cpp��while���𔲂��邽�߂ɍ쐬
	/// </summary>
	/// <param name="isGameEnd">true : �Q�[���I��, false : �I�����Ȃ�</param>
	void SetIsGameEnd(bool isGameEnd);

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

	// �Q�[�����I�����邩�ǂ���
	bool isGameEnd_;
};
