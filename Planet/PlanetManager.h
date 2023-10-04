#pragma once
#include <list>
#include <vector>
#include <memory>
#include <map>
#include "../Util/DataReaderFromUnity.h"

class PlanetBase;

class PlanetManager
{
public:
	PlanetManager(std::vector<UnityGameObject> sunData, std::vector<UnityGameObject> earthData);
	virtual ~PlanetManager();

	void Update();

	void Draw();

	std::list<std::shared_ptr<PlanetBase>> GetPlanets() const;
private:
	enum class PlanetType
	{
		SUN,
		EARTH,
	};
private:
	std::list<std::shared_ptr<PlanetBase>> pPlanets_;
	std::map<PlanetType, int> handleMap_;
};