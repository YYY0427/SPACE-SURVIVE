#pragma once
#include <DxLib.h>

// �v���g�^�C�v�錾
class SceneManager;	

// �V�[���̊��N���X
class Scene
{
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="manager">�V�[���}�l�[�W���[�̎Q��</param>
	Scene(SceneManager& manager);

	// �f�X�g���N�^
	virtual ~Scene();

	// �X�V
	virtual void Update() = 0;

	// �`��
	virtual void Draw() = 0;

	// �t�F�[�h�̍X�V
	void UpdateFade();			

	// �t�F�[�h�̕`��
	void DrawFade();

	// ���U�C�N�t�F�[�h�̕`��
	void DrawGaussFade();

	// �t�F�[�h�A�E�g�̊J�n
	void StartFadeOut();

	/// <summary>
	/// �t�F�[�h�C�������ǂ���
	/// </summary>
	/// <returns>true : �t�F�[�h�C�����Afalse : �t�F�[�h�C�����Ă��Ȃ�</returns>
	bool IsFadingIn() const;

	/// <summary>
	/// �t�F�[�h�A�E�g�����ǂ���
	/// </summary>
	/// <returns>true : �t�F�[�h�A�E�g���Afalse : �t�F�[�h�A�E�g���Ă��Ȃ�</returns>
	bool IsFadingOut() const;	

	/// <summary>
	/// �t�F�[�h�����ǂ���
	/// </summary>
	/// <returns>true : �t�F�[�h���Afalse : �t�F�[�h���ĂȂ�</returns>
	bool IsFadeing() const;	

protected:
	// �V�[���}�l�[�V���[�̎Q��
	SceneManager& manager_; 

	// �t�F�[�h�̐F
	unsigned int fadeColor_;		

	// �t�F�[�h�̖��邳
	// 0(�t�F�[�h���Ă��Ȃ�)�`255(�^����)
	int fadeBright_;	

	// �t�F�[�h���鑬�x
	// �����̏ꍇ�t�F�[�h�A�E�g�A�����̏ꍇ�t�F�[�h�C��
	int fadeSpeed_;		

	// �t�F�[�h�A�E�g�������Ȃ������ǂ���
	bool isFadeOut_;	

	int handle_;
};

