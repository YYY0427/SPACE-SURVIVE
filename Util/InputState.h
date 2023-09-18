#pragma once


/// <summary>
/// �{�^���̓��̓^�C�v
/// </summary>
enum class InputType
{
	DECISION,	// ����
	BACK,		// �߂�
	PAUSE,		// �|�[�Y�{�^��
	SLOW,		// �㏸
	FALL,		// ���~
	BOOST,		// �u�[�X�g
	UP,			// ��
	DOWN,		// ��
	RIGHT,		// �E
	LEFT,		// ��
	NUM			// �ő���̓C���f�b�N�X
};

/// <summary>
/// �p�b�h�̃g���K�[�܂��̓X�e�B�b�N�̍����E��
/// </summary>
enum class XInputType
{
	LEFT,	// �g���K�[�܂��̓X�e�B�b�N�̍�
	RIGHT,	// �g���K�[�܂��̓X�e�B�b�N�̉E
};

/// <summary>
/// �p�b�h�̃X�e�B�b�N�̓��̓^�C�v
/// </summary>
enum class XInputTypeStick
{
	NONE,			// ���͂Ȃ�
	UP,				// ��
	DOWN,			// ��
	LEFT,			// ��
	RIGHT,			// �E
	LITTLE_UP,		// ������
	LITTLE_DOWN,	// ������
	LITTLE_LEFT,	// ������
	LITTLE_RIGHT,	// �����E
	NUM				// �ő���̓C���f�b�N�X
};

/// <summary>
/// ���͑��u�J�e�S��
/// </summary>
enum class InputCategory
{
	KEYBORD,	// �L�[�{�[�h
	PAD,		// �Q�[���p�b�h
	MOUSE		// �}�E�X
};

/// <summary>
/// ���͏��
/// </summary>
struct InputInfo
{
	InputCategory cat;	// ���͑��u�J�e�S��
	int id;				// ����ID(KEY_INPUT_�`,PAD_INPUT_�`,MOUSE_INPUT_�`)
};

/// <summary>
/// ���͏�Ԃ��Ǘ�����
/// </summary>
namespace InputState
{
	// ������
	void Init();

	// ���͏��̍X�V
	void Update();

	// �{�^���������ꂽ�u�Ԃ̓��͏��̎擾
	bool IsTriggered(InputType type);

	// �{�^����������Ă���Ԃ̓��͏��̎擾
	bool IsPressed(InputType type);

	// �p�b�h�̃g���K�[�̓��͏��̎擾
	bool IsXInputTrigger(XInputType type);

	// �p�b�h�̃X�e�B�b�N�̓��͏����擾
	int IsXInputStick(XInputType stic, XInputTypeStick input);
};