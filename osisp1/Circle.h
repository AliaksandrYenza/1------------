#pragma once
#include "Shape.h"

class Circle : public Shape
{
public:
	void Circle::draw(HDC hdc)
	{
		Ellipse(hdc, startingPoint.x, startingPoint.y, finalPoint.x, finalPoint.y);
	}
};