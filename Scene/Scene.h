#pragma once

// �v���g�^�C�v�錾
class SceneManager;	// �V�[���}�l�[�W���[
class InputState;	// ���̓X�e�[�g

//���̎��_�ł�SceneManager��InputState��
//�|�C���^�������͎Q�ƂƂ��Ă��������Ȃ��B

/// <summary>
/// �V�[�����N���X
/// </summary>
class Scene
{
public:
	Scene(SceneManager& manager);
	virtual ~Scene() {};

	// �X�V
	virtual void Update(const InputState& input) = 0;

	// �`��
	virtual void Draw() = 0;

	// �t�F�[�h
	void UpdateFade();
	void DrawFade();
protected:
	// �V�[���}�l�[�V���[�ւ̎Q��
	SceneManager& manager_; 

	// �t�F�[�h
	int fadeColor_;		// �t�F�[�h����F
	int fadeBright_;	//	
	int fadeSpeed_;		// �t�F�[�h���鑬�x
};

