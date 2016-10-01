#pragma once
#include "Shape.h"

class Rect : public Shape
{
public:
	void Rect::draw(HDC hdc)
	{
		Rectangle(hdc, (long)startingPoint.x, (long)startingPoint.y, (long)finalPoint.x, (long)finalPoint.y);
	}
};