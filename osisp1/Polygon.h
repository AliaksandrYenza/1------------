#pragma once
#include "Shape.h"
#include "Line.h"

class Polygon : public Shape
{
public:
	std::vector<Line*> figures;
	Line* temp;

	Polygon::Polygon()
	{
	}

	Polygon::~Polygon()
	{}

	void addNextPoint(int x, int y)
	{
		if (figures.size == 0)
		{
			startingPoint.x = x;
			startingPoint.y = y;
		}
		temp = new Line();
		temp->setStartPosition(x, y);
	//	temp->setFinalPosition()
		figures.push_back(temp);
	}

	void Polygon::draw(HDC hdc)
	{
		for each (Line* var in figures)
		{

		}
	}
};