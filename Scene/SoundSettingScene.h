#pragma once
#include "Scene.h"

/// <summary>
/// �T�E���h�ݒ�V�[��
/// </summary>
class SoundSettingScene : public Scene
{
public:
	SoundSettingScene(SceneManager& manager);
	virtual ~SoundSettingScene();

	void Update(const InputState& input);
	void Draw();

private:
	int currentInputIndex_ = 0;
	unsigned int bgmCategoryColor = 0x000000;
	unsigned int seCategoryColor = 0x000000;
};