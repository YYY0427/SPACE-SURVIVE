#pragma once
#include <memory>

class Model;

class Background
{
public:
	Background();
	~Background();

	void Draw();

private:
	int backgroundMovieH_;
	int modelHandle_;

	std::unique_ptr<Model> pModel_;
};