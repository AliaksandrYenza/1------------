#pragma once
#include "Shape.h"

class Text : public Shape
{
public:
	std::string text;
	void Text::draw(HDC hdc)
	{
		TextOutA(hdc, startingPoint.x, startingPoint.y, text.c_str(), strlen(text.c_str()));
	}
};