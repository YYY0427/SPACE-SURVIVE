#pragma once

// �{�^���̓��̓^�C�v
enum class InputType
{
	DECISION,	// ����
	BACK,		// �߂�
	PAUSE,		// �|�[�Y
	SLOW,		// �X���[���[�V����
	BOOST,		// �u�[�X�g
	CAMERA_INVERSION,	// �J�������]
	UP,			// ��
	DOWN,		// ��
	RIGHT,		// �E
	LEFT,		// ��

	// �f�o�b�O�p
	NORMAL_ENEMY_DEBUG,	// �G���G�����R�ɓ�������
	BOSS_DETH_DEBUG,	// �{�X�̎��S

	// �{�^���̓��̓^�C�v�̍��v�l
	TOTAL_VALUE	
};

// �p�b�h�̃g���K�[�̍����E���܂��̓p�b�h�̃X�e�B�b�N�̍����E��
enum class PadLR
{
	LEFT,	// �g���K�[�܂��̓X�e�B�b�N�̍�
	RIGHT,	// �g���K�[�܂��̓X�e�B�b�N�̉E
};

// �p�b�h�̃X�e�B�b�N�̓��̓^�C�v
enum class PadStickInputType
{
	UP,				// ��
	DOWN,			// ��
	LEFT,			// ��
	RIGHT,			// �E
};

// ���͑��u�J�e�S��
enum class InputCategory
{
	KEYBORD,	// �L�[�{�[�h
	PAD,		// �Q�[���p�b�h
	MOUSE		// �}�E�X
};

// ���͏��
struct InputInfo
{
	InputCategory cat;	// ���͑��u�J�e�S��
	int id;				// ����ID(KEY_INPUT_�`,PAD_INPUT_�`,MOUSE_INPUT_�`)
};

// ���͏�Ԃ��Ǘ�����
namespace InputState
{
	// ������
	void Init();

	// ���͏��̍X�V
	// ���t���[��Update���Ă΂Ȃ��Ɠ��͏�Ԃ͍X�V����Ȃ�
	void Update();

	/// <summary>
	/// �{�^���������ꂽ�u�Ԃ̓��͏��̎擾
	/// </summary>
	/// <param name="type">�擾���������̓^�C�v</param>
	/// <returns>true : ���́Afalse : �����</returns>
	bool IsTriggered(InputType type);

	/// <summary>
	/// �{�^����������Ă���Ԃ̓��͏��̎擾
	/// </summary>
	/// <param name="type">�擾���������̓^�C�v</param>
	/// <returns>true : ���͒��Afalse : �����</returns>
	bool IsPressed(InputType type);

	/// <summary>
	/// �p�b�h�̃g���K�[�̓��͏��̎擾
	/// </summary>
	/// <param name="type">�擾���������̓^�C�v</param>
	/// <returns>true : ���͒��Afalse : �����</returns>
	bool IsPadTrigger(PadLR type);

	/// <summary>
	/// �p�b�h�̃X�e�B�b�N�̓��͏����擾
	/// </summary>
	/// <param name="stick">�X�e�B�b�N�̉E������</param>
	/// <param name="input">�X�e�B�b�N�̂ǂ̓��̓^�C�v��</param>
	/// <returns>�X�e�B�b�N�̌X�����킢(0�̂Ƃ��͓��͂Ȃ�)�@�X���Ă���p�x���傫���قǑ傫���������Ԃ��Ă���</returns>
	int IsPadStick(PadLR stic, PadStickInputType type);
};