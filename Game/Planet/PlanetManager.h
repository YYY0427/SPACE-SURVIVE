#pragma once
#include <list>
#include <vector>
#include <memory>
#include <map>

class PlanetBase;

class PlanetManager
{
public:
	PlanetManager();
	~PlanetManager();

	void Update();

	void Draw();

	const std::list<std::shared_ptr<PlanetBase>>& GetPlanets() const;
private:
	enum class PlanetType
	{
		SUN,
		EARTH,
	};
private:
	std::list<std::shared_ptr<PlanetBase>> pPlanets_;
	std::map<PlanetType, int> handleTable_;
};