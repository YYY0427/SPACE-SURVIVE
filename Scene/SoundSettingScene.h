#pragma once
#include "Scene.h"

/// <summary>
/// �T�E���h�ݒ�V�[��
/// </summary>
class SoundSettingScene : public Scene
{
public:
	// �R���X�g���N�^
	SoundSettingScene(SceneManager& manager);

	// �f�X�g���N�^
	virtual ~SoundSettingScene();

	// �X�V
	void Update(const InputState& input);

	// �`��
	void Draw();
private:
	// �T�E���h�̎��
	enum SoundType
	{
		BGM,	
		SE,
		NUM,	// �I�����������v�ł��邩
	};
private:
	// ���ݑI�𒆂̃C���f�b�N�X
	int currentSelectIndex_;
};