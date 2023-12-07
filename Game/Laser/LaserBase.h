#pragma once
#include <DxLib.h>
#include <memory>
#include "../Util/Model.h"

enum class LaserType
{
	NORMAL, 
	NORMAL_INFINITY,
	CUBE,
	REFLECT
};

class LaserBase
{
public:
	LaserBase();
	virtual ~LaserBase();

	virtual void Update() = 0;
	virtual void Draw() = 0;

	// ë∂ç›Ç∑ÇÈÇ©ämîFÇµÇƒçÌèú
	virtual void ConfirmDelete() {}

	bool IsEnabled() const;

	int GetModelHandle() const;

	virtual VECTOR GetVec() const;

	void GraduallyAlphaDelete();

	bool IsReflect() const;
	void SetIsReflect(bool isReflect);

protected:
	std::unique_ptr<Model> pModel_;

	VECTOR pos_;
	VECTOR* firePos_;
	VECTOR vec_;
	VECTOR rot_;
	VECTOR scale_;
	float alpha_;

	bool isReflect_;
	bool isEnabled_;
};