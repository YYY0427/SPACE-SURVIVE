#pragma once
#include "Scene.h"
#include <array>

class PadSettingScene : public Scene
{
public:
	PadSettingScene(SceneManager& manager);
	virtual ~PadSettingScene();

	void Update();

	void Draw();

	void DrawSwitch(bool isOn, int item);
private:
	enum class Item
	{
		PAD_SENS_X,
		PAD_SENS_Y,
		PAD_REVERSE_X,
		PAD_REVERSE_Y,
		BACK,
		TOTAL_VALUE
	};
private:
	// ‘I‘ğ’†‚Ì€–Ú
	int currentSelectItem_;

	std::array<int, static_cast<int>(Item::TOTAL_VALUE)> itemColorTable_;
};