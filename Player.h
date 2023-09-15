#pragma once
#include <DxLib.h>
#include <memory>

using namespace std;

class Model;

/// <summary>
/// プレイヤークラス
/// </summary>
class Player
{
public:
	// コンストラクタ
	Player();

	// デストラクタ
	virtual ~Player();

	// 更新
	void Update();

	// 描画
	void Draw();
private:
	shared_ptr<Model> pModel_;

	// 位置情報
	VECTOR pos_;
};