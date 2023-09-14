#pragma once
#include "Scene.h"

class InputState;

/// <summary>
/// キーコンフィグシーン
/// </summary>
class KeyConfigScene : public Scene
{
public:
	// コンストラクタ
	KeyConfigScene(SceneManager& manager, const InputState& input);

	// デストラクタ
	virtual ~KeyConfigScene();

	// 更新
	void Update(const InputState& input);

	// 描画
	void Draw();

private:
	const InputState& inputState_;

	// 現在選択中のインデックス
	int currentSelectIndex_;

	// 現在特定の入力は編集中かどうか
	bool isEditing_;
};

