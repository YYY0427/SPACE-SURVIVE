#include "EnemyBase.h"
#include "../Util/Model.h"
#include "../Util/Timer.h"

EnemyBase::EnemyBase() :
	pos_({}),
	rot_({}),
	lazerFireIntervalTimer_({}),
	lazerSpeed_(0.0f)
{
}

EnemyBase::~EnemyBase()
{
}
