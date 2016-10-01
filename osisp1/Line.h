#pragma once
#include "Shape.h"

class Line : public Shape
{
public:
	void Line::draw(HDC hdc)
	{
		MoveToEx(hdc, startingPoint.x, startingPoint.y, NULL);
		LineTo(hdc, finalPoint.x, finalPoint.y);
	}
};

