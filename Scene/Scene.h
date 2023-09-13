#pragma once
#include <DxLib.h>

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
	void UpdateFade();		// �t�F�[�h�̍X�V
	void DrawFade();		// �t�F�[�h�̕`��
	void StartFadeOut();	// �t�F�[�h�A�E�g�̊J�n
	bool IsFadingIn() const;	// �t�F�[�h�C�������ǂ���
	bool IsFadingOut() const;	// �t�F�[�h�A�E�g�����ǂ���
	bool IsFadeing() const;		// �t�F�[�h�����ǂ���
	int GetFadeBright() const;	// �t�F�[�h�̖��邳�̎擾
	void SetFadeConfig(int fadeSpeed, VECTOR fadeColor, int fadeBright);	// �t�F�[�h�̐ݒ�
protected:
	// �V�[���}�l�[�V���[�ւ̎Q��
	SceneManager& manager_; 

	// �t�F�[�h
	int fadeColor_;		// �t�F�[�h����F
	int fadeBright_;	// �t�F�[�h�̖��邳
	int fadeSpeed_;		// �t�F�[�h���鑬�x
	bool isFadeOut_;	// �t�F�[�h�A�E�g�������Ȃ������ǂ���
};

