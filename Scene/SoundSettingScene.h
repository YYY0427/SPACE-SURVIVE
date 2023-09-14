#pragma once
#include "Scene.h"

/// <summary>
/// サウンド設定シーン
/// </summary>
class SoundSettingScene : public Scene
{
public:
	// コンストラクタ
	SoundSettingScene(SceneManager& manager);

	// デストラクタ
	virtual ~SoundSettingScene();

	// 更新
	void Update(const InputState& input);

	// 描画
	void Draw();
private:
	// サウンドの種類
	enum SoundType
	{
		BGM,	
		SE,
		NUM,	// 選択肢が何個合計であるか
	};
private:
	// 現在選択中のインデックス
	int currentSelectIndex_;
};