#pragma once
#include "SceneBase.h"
#include <vector>

// �p�b�h�̐ݒ���s���V�[��
class PadSettingScene : public SceneBase
{
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="manager">�V�[���}�l�[�W���[�̎Q��</param>
	PadSettingScene(SceneManager& manager);

	// �f�X�g���N�^
	virtual ~PadSettingScene();

	// �X�V
	void Update();

	// �`��
	void Draw();

	/// <summary>
	/// �p�b�h�̔��]�̃I���A�I�t�̕\��
	/// </summary>
	/// <param name="isOn">�p�b�h�̔��]���I�����ǂ���</param>
	/// <param name="item">���ڂ̔ԍ�</param>
	void DrawPadReverse(bool isOn, int item);
private:
	// ����
	enum class Item
	{
		PAD_SENS_X,		// �p�b�h�X�e�B�b�N�̉����x
		PAD_SENS_Y,		// �p�b�h�X�e�B�b�N�̏c���x
		PAD_REVERSE_X,	// �p�b�h�X�e�B�b�N�̉����]
		PAD_REVERSE_Y,	// �p�b�h�X�e�B�b�N�̏c���]
		BACK,			// �߂�
		TOTAL_VALUE		// ���ڂ̍��v�l
	};
private:
	// �I�𒆂̍���
	int currentSelectItem_;

	// ���ڂ̕`��F�f�[�^
	std::vector<int> itemColorTable_;
};