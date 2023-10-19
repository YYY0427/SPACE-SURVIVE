#pragma once
#include <DxLib.h>

// �v���g�^�C�v�錾
class SceneManager;	

// �V�[���̊��N���X
class SceneBase
{
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="manager">�V�[���}�l�[�W���[�̎Q��</param>
	SceneBase(SceneManager& manager);

	// �f�X�g���N�^
	virtual ~SceneBase();

	// �X�V
	virtual void Update() = 0;

	// �`��
	virtual void Draw() = 0;

	// �t�F�[�h�̍X�V
	void UpdateFade();			

	/// <summary>
	/// �t�F�[�h�̕`��
	/// </summary>
	/// <param name="isPlay">true : �t�F�[�h���s��, false: �t�F�[�h���s��Ȃ�</param>
	void DrawFade(bool isPlay);

	/// <summary>
	/// ���U�C�N�t�F�[�h�̕`�� 
	/// </summary>
	/// <param name="isPlay">true : �t�F�[�h���s��, false: �t�F�[�h���s��Ȃ�</param>
	void DrawGaussFade(bool isPlay);

	// �t�F�[�h�A�E�g�̊J�n
	void StartFadeOut(int fadeBrightUpperLimitValue, int fadeSpeed = 8);

	// �t�F�[�h�C���̊J�n
	void StartFadeIn(int fadeSpeed = 8);

	/// <summary>
	/// �t�F�[�h�C�������ǂ���
	/// </summary>
	/// <returns>true : �t�F�[�h�C����, false : �t�F�[�h�C�����Ă��Ȃ�</returns>
	bool IsFadingIn() const;

	/// <summary>
	/// �t�F�[�h�A�E�g�����ǂ���
	/// </summary>
	/// <returns>true : �t�F�[�h�A�E�g��, false : �t�F�[�h�A�E�g���Ă��Ȃ�</returns>
	bool IsFadingOut() const;	

	/// <summary>
	/// �t�F�[�h�����ǂ���
	/// </summary>
	/// <returns>true : �t�F�[�h��, false : �t�F�[�h���ĂȂ�</returns>
	bool IsFadeing() const;	

	/// <summary>
	/// �t�F�[�h�A�E�g�X�^�[�g��Ƀt�F�[�h�A�E�g���ł͂Ȃ��� 
	/// </summary>
	/// <returns>true : �t�F�[�h�A�E�g�������X�^�[�g���Ă��ăt�F�[�h�A�E�g���ł͂Ȃ�,
	///			false : �t�F�[�h�A�E�g�������X�^�[�g���Ă��Ȃ�,�@�X�^�[�g���Ă��邪�t�F�[�h�A�E�g��</returns>
	bool IsStartFadeOutAfterFadingOut();

	// �t�F�[�h�̖��邳�̎擾
	int GetFadeBright() const;

	/// <summary>
	/// �t�F�[�h�̖��邳�̐ݒ�
	/// </summary>
	/// <param name="fadeBright">�ݒ肵�������邳�̒l</param>
	void SetFadeBright(int fadeBright);
protected:
	// �V�[���}�l�[�V���[�̎Q��
	SceneManager& manager_;
private:
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

	// ���U�C�N�t�F�[�h�p�̕`��O���t�B�b�N
	int gaussScreen_;

	// �t�F�[�h�̏���l
	// 0(�t�F�[�h���Ȃ�)�`255(�Ō�܂Ńt�F�[�h���s��)
	int fadeBrightUpperLimitValue_;
};

