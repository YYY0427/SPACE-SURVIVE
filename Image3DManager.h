#pragma once
#include <map>
#include <memory>
#include <vector>
#include "Util/DataReaderFromUnity.h"

class Image3D;

class Image3DManager
{
public:
	Image3DManager(std::vector<UnityGameObject> roadsData);
	virtual ~Image3DManager();

	void Draw();

	std::vector<std::shared_ptr<Image3D>> GetRoads() const;
private:
	enum class Image3DType
	{
		LOAD,
	};

private:
	std::vector<std::shared_ptr<Image3D>> pRoads_;
	std::vector<std::shared_ptr<Image3D>> pImgae3Des_;
	std::map<Image3DType, int> handleMap_;
};