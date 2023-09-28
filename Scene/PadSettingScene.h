#pragma once
#include "Scene.h"

class PadSettingScene : public Scene
{
public:
	PadSettingScene(SceneManager& manager);
	virtual ~PadSettingScene();

	void Update();

	void Draw();

private:
	enum class Item
	{
		PAD_SENS_X,
		PAD_SENS_Y,
		PAD_REVERSE_X,
		PAD_REVERSE_Y,
		TOTAL_VALUE
	};
private:

};