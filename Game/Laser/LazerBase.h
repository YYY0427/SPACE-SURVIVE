#pragma once
#include <DxLib.h>
#include <memory>
#include "../Util/Model.h"

enum class LaserType
{
	NORMAL, 
	CONTINUE_NORMAL,
	CUBE,
};

class LazerBase
{
public:
	LazerBase();
	virtual ~LazerBase();

	virtual void Update() = 0;
	virtual void Draw() = 0;

	/// <summary>
	/// レーザーを反射
	/// </summary>
	/// <param name="pos">反射するレーザーを発射する位置</param>
	/// <param name="norm">シールドの法線情報</param>
	virtual void Refrect(const VECTOR pos, const VECTOR normal){};


	void Delete();

	// カメラ内に存在するか
	// 存在しなかったら削除する
	virtual void ConfirmDelete() = 0;

	bool GetIsEnabled() const;

	int GetModelHandle() const;

	bool GetIsRefrect() const;

protected:
	std::unique_ptr<Model> pModel_;

	VECTOR pos_;
	VECTOR* firePos_;
	VECTOR vec_;
	VECTOR rot_;
	VECTOR scale_;

	bool isEnabled_;

	// 反射後のレーザーか
	bool isRefrect_;
};