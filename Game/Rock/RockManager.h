#pragma once
#include <list>
#include <vector>
#include <memory>
#include <map>
#include "../Util/DataReaderFromUnity.h"

class RockBase;
class Player;

class RockManager
{
public:
	RockManager(std::vector<UnityGameObject> rockData, std::vector<UnityGameObject> meteorData);
	virtual ~RockManager();

	void Update();
	void Draw();

	// 存在しているかの確認
	void CheckEnabled();

	std::list<std::shared_ptr<RockBase>> GetRocks();
private:
	enum class RockType
	{
		ROCK,
		METEOR,
	};
private:
	std::list<std::shared_ptr<RockBase>> pRocks_;
	std::map<RockType, int>handleTable_;	
};