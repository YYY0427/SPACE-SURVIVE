#pragma once
#include "Scene.h"
#include <vector>

// �ݒ�V�[��
class OptionScene : public Scene
{
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="manager">�V�[���}�l�[�W���[�̎Q��</param>
	OptionScene(SceneManager& manager);

	// �f�X�g���N�^
	virtual ~OptionScene();

	// �X�V
	void Update();

	// �`��
	void Draw();
private:
	// �ݒ�V�[���̍���
	enum class Item
	{
		LANGUAGE, 
		WINDOW_MODE,
		MASTER_VOLUME,
		BGM_VOLUME,	
		SE_VOLUME,
		PAD_SETTING, 
		BACK,
		/*PAD_STICK_SENS_X,
		PAD_STICK_SENS_Y,
		PAD_STICK_REVERSE_X,
		PAD_STICK_REVERSE_Y,*/
		TOTAL_VALUE			// ���ڂ̍��v�l
	};

private:
	// ���ݑI�𒆂̍���
	int currentSelectItem_;

	// ���ڂ̕`��F�f�[�^
	std::vector<int> itemColorDataTable_;

	// ���ʃA�C�R���̉摜�n���h��
	int soundIconImgHandle_;
};