#pragma once
#include <DxLib.h>
#include <memory>
#include "../Util/Model.h"

enum class LazerType
{
	NORMAL, 
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
	/// ÉåÅ[ÉUÅ[Çî≠éÀ
	/// </summary>
	/// <param name="pos">î≠éÀèÍèä</param>
	virtual void Fire(const VECTOR pos, const VECTOR vec, const VECTOR rot);

	void Refrect();

	bool GetIsEnabled() const;

	int GetModelHandle() const;

protected:
	std::unique_ptr<Model> pModel_;

	VECTOR pos_;
	VECTOR firePos_;
	VECTOR vec_;
	VECTOR rot_;
	VECTOR scale_;

	bool isEnabled_;
};