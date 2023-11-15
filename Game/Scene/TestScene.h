#pragma once
#include "SceneBase.h"
#include <memory>

// プロトタイプ宣言
class Camera;
class Warning;
class HpBar;

// テストシーン
// 色々試すようのシーン
class TestScene : public SceneBase
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="manager">シーンマネージャーの参照</param>
	TestScene(SceneManager& manager);

	// デストラクタ
	virtual ~TestScene();

	// メンバ関数ポインタの更新
	void Update();
	
	// 描画
	void Draw();

private:
	// ポインタ
	std::shared_ptr<Camera> pCamera_;
	std::shared_ptr<Warning> pWarning_;
	std::shared_ptr<HpBar> pHpBar_;
};